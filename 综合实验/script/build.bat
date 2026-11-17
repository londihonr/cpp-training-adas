@echo off
rem 构建实验1：可用 MinGW Makefiles 生成器，需在 PATH 中提供 cmake 与 mingw32-make。
rem 注意：工程所在路径必须是纯 ASCII 路径，否则 mingw32-make 会因编码问题找不到源文件。
setlocal
pushd "%~dp0.."

if not exist build md build

pushd build
cmake -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
if errorlevel 1 goto :fail
cmake --build . -- -j 4
if errorlevel 1 goto :fail
popd

popd
exit /b 0

:fail
popd
popd
exit /b 1
