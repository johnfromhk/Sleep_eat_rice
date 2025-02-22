chcp 936
:loop
del img.bmp
del input.txt
del result.txt
@echo off
if exist "%SystemRoot%\SysWOW64" path %path%;%windir%\SysNative;%SystemRoot%\SysWOW64;%~dp0

bcdedit >nul

if '%errorlevel%' NEQ '0' (goto UACPrompt) else (goto UACAdmin)
echo 请以管理员身份开始执行本程序
echo 按任意键继续
pause
:UACPrompt

%1 start "" mshta vbscript:createobject("shell.application").shellexecute("""%~0""","::",,"runas",1)(window.close)&exit

exit /B

:UACAdmin

cd /d "%~dp0"
@echo on
echo 打开干饭玩法并进入游戏后
echo 按任意键开始
pause
scp.exe -windowTitle "羊了个羊：星球"
yyy_st.exe
core.exe
click_mouse.exe -windowTitle "羊了个羊：星球" -kx 0.087362 -bx 0.107088 -ky 0.047693 -by 0.193443
pause
goto loop