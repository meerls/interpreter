main: main.c
	cc $^ -o$@ -Wall -Wextra -g -fsanitize=address
