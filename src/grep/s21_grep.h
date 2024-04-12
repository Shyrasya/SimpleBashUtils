#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 10000

typedef struct {
  int e_template;
  int i_ignore_register;
  int v_invert_search;
  int c_numb_match_lines;
  int l_match_files;
  int n_pre_numb_line;
  int h_no_pre_file_name;
  int s_ignore_errors;
  int f_file_regular;
  int o_complete_match;
  char pat_buffer[CAPACITY];
  char file_f_buffer[CAPACITY];
} flags_t;

typedef struct {
  int multi_file;
  int line_match_count;
  int file_count_lines;
} counters_t;

int find_flags(int argc, char *argv[], flags_t *flags, int *file_index);
void find_flags_extra(flags_t *flags, int argc, char *argv[]);
void print_second_part(int argc, char *argv[], flags_t *flags, int *file_index);
int print_first_part(FILE *file, char *filename, counters_t *counters,
                     flags_t *flags, regex_t *regular);
void file_inw_copy(flags_t *flags);
void print_o_flag(regmatch_t *start_end, regex_t *regular, char *buffer);

#endif