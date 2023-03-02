@echo off
zig cc -std=c99 -Wall -Wextra -Werror test.c -o test.exe && test
