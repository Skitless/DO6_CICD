FLAGS = -Wall -Wextra -Werror

all: s21_grep


s21_grep: Makefile grep_utils.h grep_utils.c
	gcc $(FLAGS) grep_utils.c -o s21_grep


check:
	clang-format -style=Google -i *.c
	clang-format -style=Google -n *.c