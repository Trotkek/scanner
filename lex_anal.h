#ifndef SCANNER_SCANNER_H
#define SCANNER_SCANNER_H

#include <stdlib.h>


/*
 * do, else, end, function,
 * global, if, integer, local, nil,
 * number, require, return, string, then, while
 * */
typedef enum tokenType {
    TOKEN_EOL,
    TOKEN_ID,

    TOKEN_DATA_INT,
    TOKEN_DATA_DOUBLE,
    TOKEN_DATA_STRING,

    TOKEN_KEYWORD_INTEGER,
    TOKEN_KEYWORD_NUMBER,
    TOKEN_KEYWORD_STRING,
    TOKEN_KEYWORD_NIL,

    TOKEN_KEYWORD_DO,
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_END,
    TOKEN_KEYWORD_FUNCTION,
    TOKEN_KEYWORD_GLOBAL,
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_LOCAL,
    TOKEN_KEYWORD_REQUIRE,
    TOKEN_KEYWORD_RETURN,
    TOKEN_KEYWORD_THEN,
    TOKEN_KEYWORD_WHILE,

    TOKEN_OP_PLUS,          // +
    TOKEN_OP_MINUS,         // -
    TOKEN_OP_MUL,           // *
    TOKEN_OP_DIV,           // /
    TOKEN_OP_INT_DIV,       // //
    TOKEN_OP_HASHTAG,       // #    string length operator
    TOKEN_OP_ASSIGN,        // =
    TOKEN_OP_CONCAT,        // ..   concat 2 strings

    TOKEN_OP_LT,            // <
    TOKEN_OP_GT,            // >
    TOKEN_OP_LE,            // <=
    TOKEN_OP_GE,            // >=
    TOKEN_OP_EQ,            // ==
    TOKEN_OP_NEQ,           // != respectively ~= in lua

    BRACKET_OPEN,           // (
    BRACKET_CLOSE,          // )
    COMMA,                  // ,
    SEMICOLON,              // ;

} tokenType_t;

typedef struct token token_t;

struct token {
    char* value;
    tokenType_t type;
    token_t *next;
};

typedef struct tok_list {
    token_t *list_head;
    token_t *list_end;
} list_t;

typedef struct tok_string {
    char *str;
    unsigned int len;
} string_t;

#endif //SCANNER_SCANNER_H