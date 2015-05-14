@echo off

if exist %1.obj del %1.obj
if exist %1.exe del %1.exe

\masm32\bin\ml /c /coff %1.asm

\masm32\bin\Link /SUBSYSTEM:CONSOLE %1.obj

del %1.obj