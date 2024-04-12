#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  int b_num_no_empty;
  int e_dollend;
  int n_num_all;
  int s_squeeze;
  int t_tab;
  int v_nonprint_char;
  int error;
} flags;

int parsing(int argc, char *argv[], char *short_options,
            struct option long_options[], flags *cur_flags, int *file_index);
int print_cur_symbol(int argc, char *argv[], flags *cur_flags, int *file_index);

void print_file_not_null(FILE *file, flags *cur_flags);

#endif