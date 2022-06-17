#ifndef RAT_STRING_MANIPULATION_H
#define RAT_STRING_MANIPULATION_H

#include <stdbool.h>
#include <stddef.h>

size_t **str_matches(char *heystack, char *needle, char **heystack_copy, int *matches_count);
bool str_contains(char *heystack, char *needle);
char *highlight_needle(char *heystack, char *needle, bool is_highlighted);

bool pattern_match(char *string, char *pattern);

#endif