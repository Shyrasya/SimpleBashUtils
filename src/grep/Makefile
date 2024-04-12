CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

all: s21_grep

s21_grep: s21_grep.o
	$(CC) $(CFLAGS) s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c s21_grep.h
	$(CC) $(CFLAGS) -c s21_grep.c

check:
	cp ../../materials/linters/.clang-format ./
	clang-format -style=Google -n *.c *.h
	clang-format -style=Google -i *.c *.h

	rm .clang-format
	
clean:
	rm -f s21_grep
	rm -f *.o
	
rebuild: clean all