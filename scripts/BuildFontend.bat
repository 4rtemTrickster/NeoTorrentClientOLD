@echo off
pyinstaller --noconsole --name NTCFrontend --distpath ../bin/Debug-windows-x86_64/ --workpath ../NTCFrontend/build --specpath ../NTCFrontend/ ../NTCFrontend/src/Main.py
xcopy /s/e "../Res" "../bin/Debug-windows-x86_64/Res"
pause