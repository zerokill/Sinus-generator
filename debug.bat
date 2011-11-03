@echo off
IF "%1"=="" GOTO kies
IF %1==usb GOTO usb
IF %1==wiggler GOTO wiggler
:kies
echo Press u if u want to debug through usb
echo Press w if u want to debug through the parralel port
CHOICE /C:UW
IF NOT ERRORLEVEL 1 GOTO abort 
IF ERRORLEVEL 2 GOTO wiggler
IF ERRORLEVEL 1 GOTO usb
:usb
cmd /c start openocd-ftd2xx.exe -f openocd_hu.cfg 
GOTO abort
:wiggler
cmd /c start OcdLibRemote --cpu ARM7 --device WIGGLER)
:abort