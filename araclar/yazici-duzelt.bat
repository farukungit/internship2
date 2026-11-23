@echo off
chcp 65001 >nul
title Yazici Duzeltme Araci
setlocal EnableDelayedExpansion

:: Yonetici yetkisi kontrolu, yoksa kendini yonetici olarak yeniden baslatir
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo Yonetici izni isteniyor...
    powershell -NoProfile -Command "Start-Process -FilePath '%~f0' -Verb RunAs"
    exit /b
)

:menu
cls
echo ==========================================
echo   YAZICI DUZELTME ARACI (cop karakter sorunu)
echo ==========================================
echo.
echo   1) Yazdirma kuyrugunu temizle (Spooler sifirla)
echo   2) Mevcut yazicilari ve portlari listele
echo   3) Secilen yaziciyi + surucusunu kaldir
echo   4) Standart TCP/IP port ekle (WSD yerine)
echo   5) Yazici ekle (surucu once kurulmus olmali)
echo   6) Test sayfasi yazdir
echo   7) HEPSI: 1 + 3 (temizle ve kaldir)
echo   0) Cikis
echo.
set /p secim=Seciminiz:

if "%secim%"=="1" goto kuyruk
if "%secim%"=="2" goto liste
if "%secim%"=="3" goto kaldir
if "%secim%"=="4" goto port
if "%secim%"=="5" goto ekle
if "%secim%"=="6" goto test
if "%secim%"=="7" goto hepsi
if "%secim%"=="0" exit /b
goto menu

:kuyruk
echo.
echo [1] Spooler durduruluyor...
net stop spooler /y >nul 2>&1
echo [1] Kuyruk dosyalari siliniyor...
del /q /f /s "%SystemRoot%\System32\spool\PRINTERS\*" >nul 2>&1
echo [1] Spooler baslatiliyor...
net start spooler >nul 2>&1
echo Tamam: kuyruk temizlendi.
if "%secim%"=="7" goto kaldir
pause
goto menu

:liste
echo.
echo --- Yazicilar ---
powershell -NoProfile -Command "Get-Printer | Format-Table Name,DriverName,PortName -AutoSize"
echo --- Portlar ---
powershell -NoProfile -Command "Get-PrinterPort | Format-Table Name,PrinterHostAddress -AutoSize"
pause
goto menu

:kaldir
echo.
powershell -NoProfile -Command "Get-Printer | Format-Table Name,DriverName,PortName -AutoSize"
set "PNAME="
set /p PNAME=Kaldirilacak yazici adi (bos = atla):
if not defined PNAME goto kaldir_son
echo Yazici ve surucu kaldiriliyor...
powershell -NoProfile -Command "$n='%PNAME%'; $p=Get-Printer -Name $n -ErrorAction SilentlyContinue; if(-not $p){Write-Host 'Yazici bulunamadi.'; exit}; $d=$p.DriverName; Remove-Printer -Name $n; Write-Host 'Yazici kaldirildi.'; net stop spooler /y | Out-Null; net start spooler | Out-Null; try{Remove-PrinterDriver -Name $d -ErrorAction Stop; Write-Host ('Surucu kaldirildi: '+$d)}catch{Write-Host ('Surucu kaldirilamadi (baska yazici kullaniyor olabilir): '+$d)}"
:kaldir_son
echo.
echo Simdi ureticinin sitesinden MODELE OZEL surucuyu (PCL6 veya PS) kurun,
echo sonra menuden 4 ve 5'i kullanin.
pause
goto menu

:hepsi
set secim=7
goto kuyruk

:port
echo.
set "IP="
set /p IP=Yazicinin IP adresi (orn 192.168.1.50):
if not defined IP goto menu
powershell -NoProfile -Command "try{Add-PrinterPort -Name 'IP_%IP%' -PrinterHostAddress '%IP%' -ErrorAction Stop; Write-Host 'Port eklendi: IP_%IP%'}catch{Write-Host $_.Exception.Message}"
pause
goto menu

:ekle
echo.
echo Kurulu suruculer:
powershell -NoProfile -Command "Get-PrinterDriver | Format-Table Name -AutoSize"
set "YNAME="
set "DNAME="
set "PORTN="
set /p YNAME=Yazici adi (istediginiz isim):
set /p DNAME=Surucu adi (yukaridaki listeden aynen):
set /p PORTN=Port adi (orn IP_192.168.1.50):
powershell -NoProfile -Command "try{Add-Printer -Name '%YNAME%' -DriverName '%DNAME%' -PortName '%PORTN%' -ErrorAction Stop; Write-Host 'Yazici eklendi.'}catch{Write-Host $_.Exception.Message}"
pause
goto menu

:test
echo.
set "TNAME="
set /p TNAME=Yazici adi:
powershell -NoProfile -Command "Get-CimInstance Win32_Printer -Filter \"Name='%TNAME%'\" | Invoke-CimMethod -MethodName PrintTestPage"
pause
goto menu
