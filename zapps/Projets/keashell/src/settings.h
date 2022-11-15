#ifndef KEASHELL_SETTINGS_H
#define KEASHELL_SETTINGS_H

// every number is the precedent * 2
typedef enum flags_t {
    FLAG_NO_INTERPRETOR = 1,
    FLAG_NO_TYPECHECK = 2,
    FLAG_PRINT_PARSER_OUTPUT = 4,
} flags_t;


typedef struct Settings_t {
    int flags;
} Settings_t;

#endif 