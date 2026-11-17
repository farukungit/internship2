@echo off
title Autodesk tam temizlik
net session >nul 2>&1
if %errorlevel% neq 0 goto :notadmin
if not exist "%~dp0autodesk_tam_temizle.ps1" goto :noscript
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0autodesk_tam_temizle.ps1"
echo.
pause
exit /b

:notadmin
echo Bu dosyayi SAG TIK, Yonetici olarak calistir ile ac.
pause
exit /b

:noscript
echo autodesk_tam_temizle.ps1 bu dosyanin yaninda bulunamadi.
echo Zip'e sag tik, Tumunu ayikla yap, sonra ayiklanan klasorden calistir.
pause
exit /b
