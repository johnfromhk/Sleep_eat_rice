chcp 936
:loop
del img.bmp
del input.txt
del result.txt
@echo off
if exist "%SystemRoot%\SysWOW64" path %path%;%windir%\SysNative;%SystemRoot%\SysWOW64;%~dp0

bcdedit >nul

if '%errorlevel%' NEQ '0' (goto UACPrompt) else (goto UACAdmin)
echo ���Թ���Ա��ݿ�ʼִ�б�����
echo �����������
pause
:UACPrompt

%1 start "" mshta vbscript:createobject("shell.application").shellexecute("""%~0""","::",,"runas",1)(window.close)&exit

exit /B

:UACAdmin

cd /d "%~dp0"
@echo on
echo �򿪸ɷ��淨��������Ϸ��
echo ���������ʼ
pause
scp.exe -windowTitle "���˸�������"
yyy_st.exe
core.exe
click_mouse.exe -windowTitle "���˸�������" -kx 0.087362 -bx 0.107088 -ky 0.047693 -by 0.193443
pause
goto loop