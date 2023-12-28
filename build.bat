@echo off
tcc -Wall -Wextra -Werror -Drun util.c -run
tcc -Wall -Wextra -Werror -Drun text.c -run
