@echo off
set VTRG_PATH=%~dp0vtrg.py

if not exist "%VTRG_PATH%" (
    echo [ERROR] vtrg.py not found in this folder.
    exit /b 1
)

:: Create a wrapper batch script in user's PATH
set INSTALL_DIR=%USERPROFILE%\AppData\Local\Programs\vantor
mkdir "%INSTALL_DIR%" >nul 2>&1

copy "%VTRG_PATH%" "%INSTALL_DIR%\vtrg.py" >nul
echo @echo off > "%INSTALL_DIR%\vtrg.bat"
echo python "%INSTALL_DIR%\vtrg.py" %%* >> "%INSTALL_DIR%\vtrg.bat"

setx PATH "%INSTALL_DIR%;%PATH%"

echo [OK] vtrg installed! Open a new terminal and run:
echo    vtrg
