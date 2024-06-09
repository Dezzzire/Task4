@echo off
setlocal
cd /d %~dp0
Debug\calc.exe %*
endlocal
