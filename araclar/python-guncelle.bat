@echo off
setlocal
title Python guncelleme
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
$ErrorActionPreference = 'Stop'
$ProgressPreference = 'SilentlyContinue'
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12

try {
    Write-Host 'En guncel kararli Python surumu araniyor...'
    $idx = (Invoke-WebRequest 'https://www.python.org/ftp/python/' -UseBasicParsing).Content
    $vers = [regex]::Matches($idx, 'href="(\d+\.\d+\.\d+)/"') |
        ForEach-Object { [version]$_.Groups[1].Value } |
        Sort-Object -Descending -Unique

    $latest = $null
    foreach ($v in $vers) {
        $u = "https://www.python.org/ftp/python/$v/python-$v-amd64.exe"
        try {
            Invoke-WebRequest $u -Method Head -UseBasicParsing | Out-Null
            $latest = $v; $url = $u; break
        } catch { }
    }
    if (-not $latest) { throw 'Guncel surum bulunamadi.' }
    Write-Host "Bulunan surum: $latest"

    # Eski Python surumlerini kaldir
    $keys = 'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\*',
            'HKLM:\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\*',
            'HKCU:\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\*'
    $old = Get-ItemProperty $keys -ErrorAction SilentlyContinue |
        Where-Object { $_.DisplayName -match '^Python \d+\.\d+\.\d+ \((32|64)-bit\)$' }

    foreach ($p in $old) {
        Write-Host "Kaldiriliyor: $($p.DisplayName)"
        if ($p.UninstallString -match '^"([^"]+)"') {
            Start-Process -FilePath $Matches[1] -ArgumentList '/uninstall', '/quiet' -Wait
        }
    }

    # En guncel surumu indir ve kur
    $exe = Join-Path $env:TEMP "python-$latest-amd64.exe"
    Write-Host 'Indiriliyor...'
    Invoke-WebRequest $url -OutFile $exe -UseBasicParsing

    Write-Host 'Kuruluyor...'
    $proc = Start-Process -FilePath $exe -Wait -PassThru -ArgumentList `
        '/quiet', 'InstallAllUsers=1', 'PrependPath=1', 'Include_launcher=1', 'Include_test=0'
    Remove-Item $exe -Force -ErrorAction SilentlyContinue

    if ($proc.ExitCode -ne 0) { throw "Kurulum hata kodu: $($proc.ExitCode)" }
    Write-Host "Tamam: Python $latest kuruldu. Yeni bir terminal acip 'python --version' ile kontrol et."
}
catch {
    Write-Host "HATA: $($_.Exception.Message)" -ForegroundColor Red
}
