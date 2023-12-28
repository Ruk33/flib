@echo off
tcc -Wall -Wextra -Werror -Drun_util util.c -run
tcc -Wall -Wextra -Werror -Drun_text text.c -run
tcc -Wall -Wextra -Werror -Drun_vecn vecn.c -run
