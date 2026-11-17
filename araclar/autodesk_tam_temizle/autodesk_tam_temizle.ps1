$ErrorActionPreference = 'SilentlyContinue'

Write-Host ''
Write-Host '=== AUTODESK TAM TEMIZLIK ===' -ForegroundColor Yellow
Write-Host 'Autodesk urunlerinin HEPSI kaldirilacak (Revit, AutoCAD, 3ds Max, Access, Lisans servisi vb.).'
Write-Host 'Kisisel dosyalarina (Belgeler, D:\revit2024, D:\Autodesk, projeler) dokunulmaz.'
Write-Host 'Lisanslar hesaba bagli oldugu icin sonra Autodesk hesabinla tekrar giris yapacaksin.'
Write-Host ''
$a = Read-Host 'Devam etmek icin EVET yaz'
if ($a -ne 'EVET') { Write-Host 'Iptal edildi.'; exit }

# --- 0) Kayit defteri yedegi
$bk = Join-Path ([Environment]::GetFolderPath('Desktop')) 'autodesk_kayit_yedek'
New-Item -ItemType Directory -Force $bk | Out-Null
reg.exe export 'HKLM\SOFTWARE\Autodesk' "$bk\hklm_autodesk.reg" /y | Out-Null
reg.exe export 'HKLM\SOFTWARE\WOW6432Node\Autodesk' "$bk\hklm_wow_autodesk.reg" /y | Out-Null
reg.exe export 'HKCU\Software\Autodesk' "$bk\hkcu_autodesk.reg" /y | Out-Null
Write-Host "[0/7] Yedek alindi: $bk"

# --- 1) Calisan Autodesk islemlerini kapat
$procs = 'Revit','acad','3dsmax','AdskAccessServiceHost','AdskAccessUIHost','AdskAccessCore','AdskIdentityManager','AdskLicensingService','AdskLicensingAgent','AutodeskDesktopApp','GenuineService','AdSSO'
Stop-Process -Name $procs -Force
Write-Host '[1/7] Islemler kapatildi'

# --- 2) Autodesk'in kendi kaldirma yardimcilari
$unRoot = Join-Path $env:ProgramData 'Autodesk\Uninstallers'
Get-ChildItem $unRoot -Directory | ForEach-Object {
    $h = Join-Path $_.FullName 'AdskUninstallHelper.exe'
    if (Test-Path $h) {
        Write-Host ("      Kaldiriliyor: " + $_.Name)
        $p = Start-Process $h -PassThru
        $null = $p.WaitForExit(900000)
    }
}
Write-Host '[2/7] Kaldirma yardimcilari bitti'

# --- 3) Yayimcisi Autodesk olan MSI paketlerini kaldir
$uninstRoots = 'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\*', 'HKLM:\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\*'
Get-ItemProperty $uninstRoots | Where-Object { $_.Publisher -like 'Autodesk*' } | ForEach-Object {
    $id = $_.PSChildName
    if ($id -match '^\{[0-9A-Fa-f-]{36}\}$') {
        Write-Host ("      msiexec /x " + $id + "  (" + $_.DisplayName + ")")
        $p = Start-Process msiexec.exe -ArgumentList ('/x ' + $id + ' /qn /norestart') -PassThru
        $null = $p.WaitForExit(600000)
    }
}
Write-Host '[3/7] MSI paketleri islendi'

# --- 4) Servisleri sil
foreach ($s in 'AdskLicensingService','AdAppMgrSvc','GenuineService') {
    sc.exe stop $s | Out-Null
    sc.exe delete $s | Out-Null
}
Write-Host '[4/7] Servisler temizlendi'

# --- 5) Yetim kalmis MSI kayitlari ve Uninstall girdileri (1606 / already installed hatasinin sebebi)
$prodRoot = 'HKLM:\SOFTWARE\Classes\Installer\Products'
$userData = 'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Installer\UserData\S-1-5-18\Products'
Get-ChildItem $prodRoot | ForEach-Object {
    $n = (Get-ItemProperty $_.PSPath).ProductName
    if ($n -match 'Autodesk|Revit|AutoCAD|3ds Max|Navisworks|Inventor') {
        Write-Host ("      Yetim kayit siliniyor: " + $n)
        Remove-Item $_.PSPath -Recurse -Force
        Remove-Item (Join-Path $userData $_.PSChildName) -Recurse -Force
    }
}
Get-ChildItem 'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall', 'HKLM:\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall' | ForEach-Object {
    $pub = (Get-ItemProperty $_.PSPath).Publisher
    if ($pub -like 'Autodesk*') { Remove-Item $_.PSPath -Recurse -Force }
}
Write-Host '[5/7] Yetim kayitlar temizlendi'

# --- 6) Klasorler
$folders = @(
    "$env:ProgramFiles\Autodesk",
    "${env:ProgramFiles(x86)}\Autodesk",
    "$env:CommonProgramFiles\Autodesk Shared",
    "${env:CommonProgramFiles(x86)}\Autodesk Shared",
    "$env:ProgramData\Autodesk",
    'C:\Autodesk',
    "$env:LOCALAPPDATA\Autodesk",
    "$env:APPDATA\Autodesk",
    'D:\Program Files\Autodesk',
    'D:\Program Files (x86)\Autodesk'
)
foreach ($f in $folders) {
    if (Test-Path $f) {
        Write-Host ("      Siliniyor: " + $f)
        Remove-Item $f -Recurse -Force
    }
}
Write-Host '[6/7] Klasorler silindi'

# --- 7) Kalan Autodesk kayit anahtarlari
Remove-Item 'HKLM:\SOFTWARE\Autodesk' -Recurse -Force
Remove-Item 'HKLM:\SOFTWARE\WOW6432Node\Autodesk' -Recurse -Force
Remove-Item 'HKCU:\Software\Autodesk' -Recurse -Force
Write-Host '[7/7] Kayit anahtarlari silindi'

Write-Host ''
Write-Host 'BITTI. Bilgisayari yeniden baslat, sonra Autodesk Access ile istedigin Revit surumunu yukle.' -ForegroundColor Green
