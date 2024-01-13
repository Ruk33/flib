@echo off

call vcvarsall x64 >NUL 2>NUL

pushd bin
cl ../util.c -Drun_util && util.exe
cl ../text.c -Drun_text && text.exe
cl ../vecn.c -Drun_vecn && vecn.exe
cl ../wqueue.c -Drun_wqueue && wqueue.exe
popd bin
