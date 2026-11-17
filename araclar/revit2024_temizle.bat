@echo off
title Revit 2024 kalinti temizleme
net session >nul 2>&1
if %errorlevel% neq 0 (
  echo Bu dosyayi SAG TIK ^> Yonetici olarak calistir ile ac.
  pause
  exit /b
)

set KEY=0A4B643700420150000007C5D0680240
set GUID={7346B4A0-2400-0510-0000-705C0D862004}
set BK=%USERPROFILE%\Desktop\revit_kayit_yedek

echo Yedek aliniyor: %BK%
mkdir "%BK%" 2>nul
reg export "HKCR\Installer\Products\%KEY%" "%BK%\1_hkcr_products.reg" /y >nul 2>&1
reg export "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Installer\UserData\S-1-5-18\Products\%KEY%" "%BK%\2_userdata_products.reg" /y >nul 2>&1
reg export "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\%GUID%" "%BK%\3_uninstall.reg" /y >nul 2>&1

echo Kayit anahtarlari siliniyor...
reg delete "HKCR\Installer\Products\%KEY%" /f >nul 2>&1
reg delete "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Installer\UserData\S-1-5-18\Products\%KEY%" /f >nul 2>&1
reg delete "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\%GUID%" /f >nul 2>&1

echo Kalinti klasorler siliniyor...
if exist "C:\Program Files\Autodesk\Revit 2024" rmdir /s /q "C:\Program Files\Autodesk\Revit 2024"
if exist "D:\Program Files\Autodesk\Revit 2024" rmdir /s /q "D:\Program Files\Autodesk\Revit 2024"
if exist "D:\Autodesk\Revit 2024" rmdir /s /q "D:\Autodesk\Revit 2024"

echo.
echo Bitti. Simdi bilgisayari yeniden baslat, sonra Revit 2024'u yukle.
pause
