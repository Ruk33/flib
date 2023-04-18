@echo off
tcc -Wall -Wextra -Werror test.c flib.c -o test.exe && test
