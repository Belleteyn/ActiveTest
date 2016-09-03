@echo off
setlocal
set targetDir=%~dp0
set build=--release
set app=SMSClient.exe
set windeployqt_path=D:\Develop\SDK\Qt\Qt5.5\5.5\msvc2013\bin
%windeployqt_path%\windeployqt.exe %build% --compiler-runtime %targetDir%%app%