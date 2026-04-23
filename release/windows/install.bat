@echo off
echo ----------------------------------------
echo        Installing ShrijiLang v1.0
echo ----------------------------------------

set INSTALL_DIR=%USERPROFILE%\ShrijiLang
set DESKTOP=%USERPROFILE%\Desktop

echo Creating directory...
mkdir "%INSTALL_DIR%" >nul 2>&1
mkdir "%INSTALL_DIR%\bin" >nul 2>&1

echo Copying files...
copy bin\shrijilang.exe "%INSTALL_DIR%\bin\" >nul
copy bin\shrijilang.ico "%INSTALL_DIR%\bin\" >nul

echo Creating desktop shortcut...

echo Set oWS = WScript.CreateObject("WScript.Shell") > temp.vbs
echo sLinkFile = "%DESKTOP%\ShrijiLang.lnk" >> temp.vbs
echo Set oLink = oWS.CreateShortcut(sLinkFile) >> temp.vbs
echo oLink.TargetPath = "%INSTALL_DIR%\bin\shrijilang.exe" >> temp.vbs
echo oLink.WorkingDirectory = "%INSTALL_DIR%\bin" >> temp.vbs
echo oLink.IconLocation = "%INSTALL_DIR%\bin\shrijilang.ico" >> temp.vbs
echo oLink.Save >> temp.vbs

cscript temp.vbs >nul
del temp.vbs

echo.
echo Installation complete!
echo.

echo Shortcut created on Desktop!
echo.

echo Run using:
echo %INSTALL_DIR%\bin\shrijilang.exe

echo.
pause
