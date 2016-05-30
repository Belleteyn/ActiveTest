@echo off
setlocal
set targetDir=%~dp0
set build=--release
set app=ActiveTest.exe
set windeployqt_path=D:\develop\sdk\qt\win32\5.6\msvc2015\bin
%windeployqt_path%\windeployqt.exe %build% --compiler-runtime %targetDir%%app%