FLAGS = -Wall -Wextra -Werror

all: s21_cat


s21_cat: Makefile cat_utils.h main.c
	gcc $(FLAGS) main.c -o s21_cat
	# gcc main.c -o s21_cat


check:
	clang-format -style=Google -i *.c
	clang-format -style=Google -n *.c