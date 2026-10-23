@echo off
setlocal enabledelayedexpansion

echo ========================================================
echo   Hercule Robotic Arm - GitHub Sync Tool
echo ========================================================
echo.

:: Navigate to script directory
cd /d "%~dp0"
echo [OK] Project directory: %CD%

echo.
echo [1/3] Checking Git repository...
if not exist ".git" (
    git init
    git branch -M main
    echo [OK] Initialized Git repository.
)

echo.
echo [2/3] Staging files and creating commit...
git add .
git commit -m "feat: initial release of Hercule 6-DOF Robotic Arm platform" 2>nul

echo.
echo ========================================================
echo [3/3] Push to Remote Repository
echo ========================================================
echo If you created a new repository on GitHub (https://github.com/new):
echo Paste your repository URL below.
echo.
set /p REPO_URL="Enter your GitHub repository URL: "

if "%REPO_URL%"=="" (
    set "REPO_URL=https://github.com/salemyoussef/Hercule-Robotic-Arm.git"
)

echo.
echo Linking to: %REPO_URL%
git remote remove origin 2>nul
git remote add origin %REPO_URL%
git push -u origin main --force

if !errorlevel! equ 0 (
    echo.
    echo ========================================================
    echo [SUCCESS] Your robotic arm project is now live on GitHub!
    echo ========================================================
) else (
    echo.
    echo [ERROR] Push failed. 
    echo Please verify that the repository exists on GitHub and that you have permissions.
)

echo.
pause
