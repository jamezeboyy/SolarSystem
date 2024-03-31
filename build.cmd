@echo off

REM Check if Python is installed

python --version > nul 2>&1
if %errorlevel% neq 0 (
    echo Python is not installed. Please install Python and try again.
    exit /b 1
)

REM python -m venv env
REM env\Script\activate.bat


REM Install requirements using pip
pip install -r requirements.txt
if %errorlevel% neq 0 (
    echo Failed to install requirements. Please check if pip and the requirements.txt file are correctly set up.
    exit /b 1
)

echo Requirements installation successful.

REM conan profile detect
conan install .


exit /b 0