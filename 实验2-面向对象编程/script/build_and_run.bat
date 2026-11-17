@echo off
rem 一键构建并运行需求1的测试防护网。
setlocal
call "%~dp0build.bat"
if errorlevel 1 (
    echo There was an error during the build process.
    exit /b 1
)

pushd "%~dp0.."
"build\tests\training_main.exe"
set RC=%errorlevel%
popd
exit /b %RC%
