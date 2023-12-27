@echo off
tcc -Wall -Wextra -Werror test.c flib.c -run
tcc -Wall -Wextra -Werror -Drun text.c -run
