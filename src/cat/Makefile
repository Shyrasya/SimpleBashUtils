CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

all: s21_cat

s21_cat: s21_cat.o
	$(CC) $(CFLAGS) s21_cat.o -o s21_cat

s21_cat.o: s21_cat.c s21_cat.h
	$(CC) $(CFLAGS) -c s21_cat.c

check:
	cp ../../materials/linters/.clang-format ./
	clang-format -style=Google -n *.c *.h
	clang-format -style=Google -i *.c *.h
	
	rm .clang-format
	
clean:
	rm -f s21_cat
	rm -f *.o
	
rebuild: clean all
