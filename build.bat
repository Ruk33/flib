@echo off
zig cc -Wall -Wextra -Werror test.c -o test.exe && test
