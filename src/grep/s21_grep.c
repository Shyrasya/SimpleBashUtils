#include "s21_grep.h"

int main(int argc, char *argv[]) {
  int error = 0, file_index = 0;
  if (argc > 1) {
    flags_t flags = {0};
    if (find_flags(argc, argv, &flags, &file_index))
      error = 1;
    else {
      print_second_part(argc, argv, &flags, &file_index);
    }
  }
  return error;
}

int find_flags(int argc, char *argv[], flags_t *flags, int *file_index) {
  int act_flag = 0, error = 0;
  while ((act_flag = getopt_long(argc, argv, "e:ivclnf:hso", NULL, 0)) != -1) {
    switch (act_flag) {
      case 'e':
        flags->e_template = 1;
        strcat(flags->pat_buffer, optarg);
        strcat(flags->pat_buffer, "|");
        break;
      case 'i':
        flags->i_ignore_register = 1;
        break;
      case 'v':
        flags->v_invert_search = 1;
        break;
      case 'c':
        flags->c_numb_match_lines = 1;
        break;
      case 'l':
        flags->l_match_files = 1;
        break;
      case 'n':
        flags->n_pre_numb_line = 1;
        break;
      case 'f':
        flags->f_file_regular = 1;
        file_inw_copy(flags);
        break;
      case 'h':
        flags->h_no_pre_file_name = 1;
        break;
      case 's':
        flags->s_ignore_errors = 1;
        break;
      case 'o':
        flags->o_complete_match = 1;
        break;
      default:
        error = 1;
        break;
    }
  }
  find_flags_extra(flags, argc, argv);
  *file_index = optind;
  return error;
}

void find_flags_extra(flags_t *flags, int argc, char *argv[]) {
  if (!flags->e_template && !flags->f_file_regular) {
    if (argc > optind) strcat(flags->pat_buffer, argv[optind]);
    optind++;
  }
  if (flags->e_template || flags->f_file_regular) {
    flags->pat_buffer[strlen(flags->pat_buffer) - 1] = '\0';
  }
  if (flags->v_invert_search && flags->o_complete_match) {
    flags->o_complete_match = 0;
  }
}

void print_second_part(int argc, char *argv[], flags_t *flags,
                       int *file_index) {
  regex_t regular;
  counters_t counters = {0};
  counters.multi_file = (*file_index + 1 < argc);
  int return_match = 0;

  int r_extend = REG_EXTENDED;
  if (flags->i_ignore_register) {
    r_extend |= REG_ICASE;
  }
  regcomp(&regular, flags->pat_buffer, r_extend);
  for (int i = *file_index; (i < argc); i++) {
    char *filename = argv[i];
    FILE *file = fopen(filename, "r");
    if (file) {
      int match = print_first_part(file, filename, &counters, flags, &regular);
      return_match = match;
      if (flags->c_numb_match_lines) {
        if (counters.multi_file && !flags->h_no_pre_file_name) {
          printf("%s:", filename);
        }
        if (!return_match && flags->l_match_files)
          printf("1\n");
        else
          printf("%d\n", counters.line_match_count);
      }
      if (!return_match && flags->l_match_files) printf("%s\n", filename);
      counters.line_match_count = 0;
      counters.file_count_lines = 0;
      fclose(file);
    } else {
      if (!flags->s_ignore_errors) {
        fprintf(stderr, "%s: No such file or directory\n", filename);
        continue;
      }
    }
  }

  regfree(&regular);
}

int print_first_part(FILE *file, char *filename, counters_t *counters,
                     flags_t *flags, regex_t *regular) {
  int return_match = 0;
  char buffer[CAPACITY];
  regmatch_t start_end;
  while (fgets(buffer, CAPACITY, file) != NULL) {
    int match = regexec(regular, buffer, 1, &start_end, 0);
    if (flags->v_invert_search) {
      match = !match;
    }
    if (!match) return_match++;
    if (flags->n_pre_numb_line) {
      counters->file_count_lines++;
    }
    if (flags->c_numb_match_lines && !match) {
      counters->line_match_count++;
    }
    if (((!flags->c_numb_match_lines && !flags->l_match_files)) && !match) {
      if (!match && counters->multi_file) {
        if (!flags->h_no_pre_file_name) printf("%s:", filename);
      }
      if (!match && flags->n_pre_numb_line &&
          (!flags->l_match_files || !flags->c_numb_match_lines)) {
        printf("%d:", counters->file_count_lines);
      }
      if (!match && !flags->l_match_files && !flags->c_numb_match_lines &&
          !flags->o_complete_match) {
        printf("%s", buffer);
      }
      if (!match && flags->o_complete_match && !flags->c_numb_match_lines &&
          !flags->l_match_files) {
        print_o_flag(&start_end, regular, buffer);
      }
      if ((buffer[strlen(buffer) - 1] != '\n')) printf("\n");
    }
  }
  return !return_match;
}

void file_inw_copy(flags_t *flags) {
  FILE *file_name;
  file_name = fopen(optarg, "r");
  if (file_name != NULL) {
    while (!feof(file_name)) {
      if (fgets(flags->file_f_buffer, CAPACITY, file_name) != NULL) {
        if (flags->file_f_buffer[strlen(flags->file_f_buffer) - 1] == '\n' &&
            strlen(flags->file_f_buffer) - 1 != 0) {
          flags->file_f_buffer[strlen(flags->file_f_buffer) - 1] = '\0';
        }
        strcat(flags->pat_buffer, flags->file_f_buffer);
        strcat(flags->pat_buffer, "|");
      }
    }
    fclose(file_name);
  } else {
    fprintf(stderr, "%s: No such file or directory\n", optarg);
  }
}

void print_o_flag(regmatch_t *start_end, regex_t *regular, char *buffer) {
  char *point_match = buffer;
  while ((regexec(regular, point_match, 1, start_end, 0) == 0)) {
    printf("%.*s\n", (int)(start_end->rm_eo - start_end->rm_so),
           point_match + start_end->rm_so);
    point_match = point_match + start_end->rm_eo;
  }
}
