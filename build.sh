#!/bin/bash
tcc -Wall -Wextra -Werror test.c flib.c -o test && ./test