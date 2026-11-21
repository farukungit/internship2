@echo off
setlocal
title SolidWorks tam temizlik
net session >nul 2>&1
if %errorlevel% neq 0 (
  echo Yonetici izni isteniyor...
  powershell -NoProfile -Command "Start-Process -FilePath '%~f0' -Verb RunAs"
  exit /b
)
powershell -NoProfile -ExecutionPolicy Bypass -Command "iex ((Get-Content -Raw -LiteralPath '%~f0') -replace '(?s)^.*#PS_START','')"
echo.
pause
exit /b
#PS_START
$ErrorActionPreference = 'SilentlyContinue'
$ProgressPreference = 'SilentlyContinue'
$rx = 'solidworks|solidnetwork|edrawings'

Write-Host ''
Write-Host '=== SOLIDWORKS TAM TEMIZLIK (2024 ve tum turevleri) ===' -ForegroundColor Yellow
Write-Host 'Kaldirilacaklar: SolidWorks (tum surumler), eDrawings, PDM, Electrical, Visualize,'
Write-Host 'Composer, CAM, Simulation, Flow Simulation, Plastics, Inspection, Installation Manager,'
Write-Host 'SolidNetWork lisans yoneticisi, servisler, gorevler, kayit defteri, kalinti klasorler.'
Write-Host 'Dokunulmayacaklar: modelleriniz (.sldprt/.sldasm/.slddrw), SOLIDWORKS Data, SOLIDWORKS Downloads,'
Write-Host 'PDM kasa gorunumleri, Belgeler klasoru.'
Write-Host ''
$a = Read-Host 'Devam etmek icin EVET yaz'
if ($a -ne 'EVET') { Write-Host 'Iptal edildi.'; exit }

$desk = [Environment]::GetFolderPath('Desktop')
$bk = Join-Path $desk 'solidworks_kayit_yedek'
$log = Join-Path $desk 'solidworks_temizlik_log.txt'
Start-Transcript -Path $log -Force | Out-Null

# --- 0) Kayit defteri yedegi
New-Item -ItemType Directory -Force $bk | Out-Null
$exp = @{
  'hklm_solidworks'     = 'HKLM\SOFTWARE\SolidWorks'
  'hklm_wow_solidworks' = 'HKLM\SOFTWARE\WOW6432Node\SolidWorks'
  'hklm_edrawings'      = 'HKLM\SOFTWARE\eDrawings'
  'hklm_wow_edrawings'  = 'HKLM\SOFTWARE\WOW6432Node\eDrawings'
  'hkcu_solidworks'     = 'HKCU\Software\SolidWorks'
  'hkcu_edrawings'      = 'HKCU\Software\eDrawings'
}
foreach ($k in $exp.Keys) { reg.exe export $exp[$k] "$bk\$k.reg" /y | Out-Null }
Write-Host "[0/8] Yedek alindi: $bk"

# --- 1) Calisan islemleri kapat
Get-Process | Where-Object {
    $_.Name -match '^(sld|edrawings)' -or ($_.Path -and $_.Path -match 'SOLIDWORKS|SolidWorks|eDrawings|SolidNetWork')
} | Stop-Process -Force
Write-Host '[1/8] Islemler kapatildi'

# --- 2) Servisler ve zamanlanmis gorevler
Get-CimInstance Win32_Service | Where-Object {
    $_.Name -match $rx -or $_.DisplayName -match $rx -or $_.PathName -match $rx
} | ForEach-Object {
    Write-Host ('      Servis siliniyor: ' + $_.DisplayName)
    sc.exe stop $_.Name | Out-Null
    sc.exe delete $_.Name | Out-Null
}
Get-ScheduledTask | Where-Object { $_.TaskName -match $rx -or $_.TaskPath -match $rx } | ForEach-Object {
    Write-Host ('      Gorev siliniyor: ' + $_.TaskName)
    Unregister-ScheduledTask -TaskName $_.TaskName -TaskPath $_.TaskPath -Confirm:$false
}
Write-Host '[2/8] Servisler ve gorevler temizlendi'

# --- 3) Programlari kaldir (MSI + sessiz kaldirma komutu olanlar)
$uninstRoots = 'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\*', 'HKLM:\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\*'
for ($pass = 1; $pass -le 2; $pass++) {
    Get-ItemProperty $uninstRoots | Where-Object { $_.DisplayName -match $rx } | Sort-Object DisplayName -Descending | ForEach-Object {
        $id = $_.PSChildName
        Write-Host ("      Kaldiriliyor (tur $pass): " + $_.DisplayName)
        if ($id -match '^\{[0-9A-Fa-f-]{36}\}$') {
            $p = Start-Process msiexec.exe -ArgumentList ('/x ' + $id + ' /qn /norestart REBOOT=ReallySuppress') -PassThru
            $null = $p.WaitForExit(900000)
        }
        elseif ($_.QuietUninstallString) {
            $p = Start-Process cmd.exe -ArgumentList ('/c ' + $_.QuietUninstallString) -PassThru
            $null = $p.WaitForExit(900000)
        }
        else {
            Write-Host '        (sessiz kaldirma yok, kayit ve klasor asamasinda silinecek)'
        }
    }
}
Write-Host '[3/8] Kaldirma bitti'

# --- 4) Yetim MSI kayitlari ve Uninstall girdileri (kurulumun "zaten yuklu" demesinin sebebi)
$prodRoot = 'HKLM:\SOFTWARE\Classes\Installer\Products'
$userData = 'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Installer\UserData\S-1-5-18\Products'
Get-ChildItem $prodRoot | ForEach-Object {
    $n = (Get-ItemProperty $_.PSPath).ProductName
    if ($n -match $rx) {
        Write-Host ('      Yetim kayit siliniyor: ' + $n)
        Remove-Item $_.PSPath -Recurse -Force
        Remove-Item (Join-Path $userData $_.PSChildName) -Recurse -Force
    }
}
Get-ChildItem $userData | ForEach-Object {
    $dn = (Get-ItemProperty (Join-Path $_.PSPath 'InstallProperties')).DisplayName
    if ($dn -match $rx) {
        Write-Host ('      Yetim UserData siliniyor: ' + $dn)
        Remove-Item (Join-Path $prodRoot $_.PSChildName) -Recurse -Force
        Remove-Item $_.PSPath -Recurse -Force
    }
}
Get-ChildItem 'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall', 'HKLM:\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall' | ForEach-Object {
    $dn = (Get-ItemProperty $_.PSPath).DisplayName
    if ($dn -match $rx) { Remove-Item $_.PSPath -Recurse -Force }
}
Write-Host '[4/8] Yetim kayitlar temizlendi'

# --- 5) Klasorler
$drives = Get-CimInstance Win32_LogicalDisk | Where-Object { $_.DriveType -eq 3 } | ForEach-Object { $_.DeviceID + '\' }
$parents = @()
foreach ($d in $drives) {
    $parents += $d + 'Program Files'
    $parents += $d + 'Program Files (x86)'
    $parents += $d + 'Program Files\Common Files'
    $parents += $d + 'Program Files (x86)\Common Files'
}
$parents += $env:ProgramData
$parents += (Join-Path $env:ProgramData 'Microsoft\Windows\Start Menu\Programs')
foreach ($u in (Get-ChildItem 'C:\Users' -Directory)) {
    $parents += (Join-Path $u.FullName 'AppData\Roaming')
    $parents += (Join-Path $u.FullName 'AppData\Local')
    $parents += (Join-Path $u.FullName 'AppData\LocalLow')
    $parents += (Join-Path $u.FullName 'AppData\Roaming\Microsoft\Windows\Start Menu\Programs')
}
$targets = @()
foreach ($p in ($parents | Select-Object -Unique)) {
    foreach ($pat in 'SOLIDWORKS*', 'SolidNetWork*', 'eDrawings*') {
        $targets += Get-ChildItem -LiteralPath $p -Directory -Filter $pat | Where-Object { $_.Name -notmatch '^SOLIDWORKS (Data|Downloads)$' }
    }
}
foreach ($d in $drives) {
    foreach ($n in 'SOLIDWORKS Corp', 'SOLIDWORKS Installation Manager') {
        $t = Join-Path $d $n
        if (Test-Path -LiteralPath $t) { $targets += Get-Item -LiteralPath $t }
    }
}
foreach ($t in ($targets | Sort-Object FullName -Unique)) {
    Write-Host ('      Siliniyor: ' + $t.FullName)
    Remove-Item -LiteralPath $t.FullName -Recurse -Force
    if (Test-Path -LiteralPath $t.FullName) { cmd.exe /c ('rmdir /s /q "' + $t.FullName + '"') | Out-Null }
}
Get-ChildItem 'C:\Users\Public\Desktop', ([Environment]::GetFolderPath('Desktop')) -Filter '*.lnk' | Where-Object { $_.Name -match $rx } | Remove-Item -Force
Write-Host '[5/8] Klasorler temizlendi'

# --- 6) Kayit defteri: yazilim anahtarlari ve sinif kayitlari
$regKeys = @(
    'HKLM:\SOFTWARE\SolidWorks', 'HKLM:\SOFTWARE\WOW6432Node\SolidWorks',
    'HKLM:\SOFTWARE\SolidWorks Corp', 'HKLM:\SOFTWARE\WOW6432Node\SolidWorks Corp',
    'HKLM:\SOFTWARE\eDrawings', 'HKLM:\SOFTWARE\WOW6432Node\eDrawings',
    'HKCU:\Software\SolidWorks', 'HKCU:\Software\SolidWorks Corp', 'HKCU:\Software\eDrawings'
)
foreach ($k in $regKeys) {
    if (Test-Path $k) { Write-Host ('      Siliniyor: ' + $k); Remove-Item $k -Recurse -Force }
}
Get-ChildItem 'HKLM:\SOFTWARE\Classes' | Where-Object { $_.PSChildName -match '^(sldworks|solidworks|edrawings|sldmonitor)' } | ForEach-Object {
    Remove-Item $_.PSPath -Recurse -Force
}
Write-Host '[6/8] Kayit defteri temizlendi'

# --- 7) Kontrol
Write-Host ''
Write-Host '[7/8] Kontrol' -ForegroundColor Yellow
$left = Get-ItemProperty $uninstRoots | Where-Object { $_.DisplayName -match $rx }
$leftSvc = Get-CimInstance Win32_Service | Where-Object { $_.Name -match $rx -or $_.DisplayName -match $rx }
$leftDir = $targets | Where-Object { Test-Path -LiteralPath $_.FullName }
if (-not $left -and -not $leftSvc -and -not $leftDir) {
    Write-Host '      Kalinti bulunamadi. Temiz.' -ForegroundColor Green
} else {
    foreach ($x in $left)    { Write-Host ('      Program kaldi: ' + $x.DisplayName) -ForegroundColor Red }
    foreach ($x in $leftSvc) { Write-Host ('      Servis kaldi: ' + $x.DisplayName) -ForegroundColor Red }
    foreach ($x in $leftDir) { Write-Host ('      Klasor kaldi: ' + $x.FullName) -ForegroundColor Red }
    Write-Host '      Bilgisayari yeniden baslat ve bu dosyayi bir kez daha calistir.'
}

Write-Host ''
Write-Host '[8/8] Bitti. Bilgisayari yeniden baslat, sonra SolidWorks 2024 kurulumunu yap.' -ForegroundColor Yellow
Write-Host "Kayit yedegi: $bk"
Write-Host "Log: $log"
Stop-Transcript | Out-Null
