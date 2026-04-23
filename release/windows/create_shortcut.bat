@echo off

set INSTALL_DIR=%USERPROFILE%\ShrijiLang
set DESKTOP=%USERPROFILE%\Desktop

echo Creating shortcut...

echo Set oWS = WScript.CreateObject("WScript.Shell") > temp.vbs
echo sLinkFile = "%DESKTOP%\ShrijiLang.lnk" >> temp.vbs
echo Set oLink = oWS.CreateShortcut(sLinkFile) >> temp.vbs
echo oLink.TargetPath = "%INSTALL_DIR%\bin\shrijilang.exe" >> temp.vbs
echo oLink.WorkingDirectory = "%INSTALL_DIR%\bin" >> temp.vbs
echo oLink.Save >> temp.vbs

cscript temp.vbs >nul
del temp.vbs

echo.
echo Shortcut created on Desktop!
pause
