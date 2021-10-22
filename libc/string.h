#ifndef STRINGS_H
#define STRINGS_H

#include "../cpu/type.h"

void int_to_ascii(s32 n, char str[]);
void hex_to_ascii(s32 n, char str[]);
void reverse(char s[]);
s32 strlen(char s[]);
s32 strcmp(char s1[], char s2[]);
void backspace(char s[]);
void append(char s[], char n);

#endif
