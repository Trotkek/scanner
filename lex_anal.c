#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "lex_anal.h"
//#include "error.h"


int string_len = 0; // string debugging
list_t *tok_list;   //token list

bool first_num; // TODO neco inteligentnejsiho maybe? lmao

void ragequit (string_t *s, int errcode)
{
    if (s != NULL) free(s->str);
    while (tok_list->list_head != NULL) {
        tok_list->list_end = tok_list->list_head->next;
        free(tok_list->list_head->value);
        free(tok_list->list_head);
        tok_list->list_head = tok_list->list_end;
    }
    exit (errcode);
}


void create_token(token_t *token, char t_type, list_t *active_list, char *val)
{
    if ((token = malloc(sizeof(token_t))) != NULL) {
        token->type = t_type;
        token->value = val;
        if (active_list->list_head == NULL) {
            active_list->list_head = active_list->list_end = token;
        } else {
            active_list->list_end->next = token;
            active_list->list_end = token;
        }
    } else {    //allocating memory for token failed
        printf("reeeee\n"); //plus error code
        ragequit(NULL, 99);
    }
}

void restart_string(string_t *string_tmp)
{
    if((string_tmp->str = malloc(sizeof(char))) == NULL) {

    } else {
        string_tmp->len = 0;
        string_tmp->str[0] = '\0';
    }
    first_num = true;
}

void append_to_string (string_t *string_tmp, char c)
{
    unsigned int new_string_size = sizeof(char) * (string_tmp->len + 1);

    string_tmp->str = realloc(string_tmp->str, new_string_size);
    if (string_tmp->str == NULL) {
        ragequit(string_tmp, 99);
        //todo return error
    }
    string_tmp->str[string_tmp->len] = c;
    string_tmp->len++;
    string_tmp->str[string_tmp->len] = '\0';

    string_len++;       //TODO debugging tool remove after
    first_num = false;
}

int next_token()
{
    char c;
    token_t *token = NULL;
    string_t str;
    string_t *string = &str;

    restart_string(string);

    while (1) {
        c = (char) getchar();

        if (isspace(c)) {
            continue;
        } else if (c == '\n') {
            create_token(token, TOKEN_EOL, tok_list, NULL);
            //deal with this shit newline
        } else if (c == '+') {
            create_token(token, TOKEN_OP_PLUS, tok_list, NULL);
        } else if (c == '-') {
            if ((c = (char) getchar()) == '-') {
                if ((c = (char) getchar()) == '[') {
                    if ((c = (char) getchar()) == '[') {
                        while (1){
                            if ((c = (char) getchar()) == ']') {
                                if ((c = (char) getchar()) == ']') {
                                    break;
                                }
                            }
                        }
                    } else {
                        while ((c = (char) getchar()) != '\n');
                    }
                } else {
                    while ((c = (char) getchar()) != '\n');
                }
            }  else {
                ungetc(c, stdin);      //give it back
                create_token(token, TOKEN_OP_MINUS, tok_list, NULL);
            }
        } else if (c == '*') {
            create_token(token, TOKEN_OP_MUL, tok_list, NULL);
        } else if (c == '/') {
            if ((c = (char) getchar()) == '/') {
                create_token(token, TOKEN_OP_INT_DIV, tok_list, NULL);
            } else {
                ungetc(c, stdin);
                create_token(token, TOKEN_OP_DIV, tok_list, NULL);
            }

        } else if (c == '#') {
            create_token(token, TOKEN_OP_HASHTAG, tok_list, NULL);
        } else if (c == '<') {
            if ((c = (char) getchar()) == '=') {
                create_token(token, TOKEN_OP_LE, tok_list, NULL);
            } else {
                create_token(token, TOKEN_OP_LT, tok_list, NULL);
                ungetc(c, stdin);
            }
        } else if (c == '>') {
            if ((c = (char) getchar()) == '=') {
                create_token(token, TOKEN_OP_GE, tok_list, NULL);
            } else {
                create_token(token, TOKEN_OP_GT, tok_list, NULL);
                ungetc(c, stdin);
            }
        } else if (c == '~') {
            if ((c = (char) getchar()) == '=') {
                create_token(token, TOKEN_OP_NEQ, tok_list, NULL);
            } else {
                ragequit(str, 1)
            }       //should probably deal with this
        } else if (c == '=') {
            if ((c = (char) getchar()) == '=') {
                create_token(token, TOKEN_OP_EQ, tok_list, NULL);
            } else {
                ungetc(c, stdin);
                create_token(token, TOKEN_OP_ASSIGN, tok_list, NULL);
            }       //the fuck happens here TODO dis
        } else if(c == ';') {
            create_token(token, SEMICOLON, tok_list, NULL);
        } else if (c == ',') {
            create_token(token, COMMA, tok_list, NULL);
        } else if (c == '(') {
            create_token(token, BRACKET_OPEN, tok_list, NULL);
        } else if (c == ')') {
            create_token(token, BRACKET_CLOSE, tok_list, NULL);
        } else if (c == '.') {
            if ((c = (char) getchar()) == '.') {
                create_token(token, TOKEN_OP_CONCAT, tok_list, NULL);
            } else {
                ragequit(str, 2);
            }
        } else if (c == '"') { // string
            restart_string(string);

            while ((c = (char) getchar()) != '"') {

                if (c == '\n') {
                    ragequit(str, 2);
                } else if (c == EOF) {
                    ragequit(str, 2);
                } else if (c < ' ') {
                    ragequit(str, 2);
                } else {
                    append_to_string(string, c);
                    if (c == '\\') {
                        c = (char) getchar();
                        if (c == '"' || c == 'n' || c == 't' || c == '\\') {
                            append_to_string(string, c);
                        } else if (isnum(c)){
                            char ddd[3];
                            char *ptr;
                            long num;
                            ddd[0] = c;
                            ddd[1] = (char) getchar();
                            ddd[2] = (char) getchar();
                            num = strtol(ddd, &ptr, 10);
                            if (num < 1 || num > 255) {
                                ragequit(str, 2);
                            } else {
                                append_to_string(string, c);
                                append_to_string(string, ddd[1]);
                                append_to_string(string, ddd[2]);
                            }
                        } else {
                            ragequit(str, 2);
                        }
                    }
                }


                /*
                if (c == '\\') {    //special needs string :(
                    if ((c = (char) getc()) == '\\');
                }
                */
            }
            create_token(token, TOKEN_DATA_STRING, tok_list, string->str);

        } else if (isdigit(c)) { // numba
            restart_string(string);

            bool has_exp = false;
            bool float_num = false;

            while (1) {
                c = (char) getchar();
                if (c == '.') {
                    if (float_num) {
                        ragequit(str, 2);
                        //todo can't have 2 dots
                    } else if (has_exp) {
                        ragequit(str, 2);
                        //todo can't have dot after exponent
                    } else {
                        append_to_string(string, c);
                        float_num = true;
                        c = (char) getchar();
                        if (isdigit(c)) {
                            append_to_string(string, c);
                        } else {
                            ragequit(str, 2);
                            // TODO invalid float (at least 1 number after decimal point)
                        }
                    }

                } else if ((c == 'e') || (c == 'E')) {
                    if (has_exp) {
                        ragequit(str, 2);
                        //can't have exponent twice
                    } else {
                        append_to_string(string, c);
                        has_exp = true;
                    }
                    c = (char) getchar();
                    if ((c == '+') || (c == '-')) {
                        append_to_string(string, c);
                    } else if (isdigit(c)) {
                        append_to_string(string, c);
                    } else {
                        ragequit(str, 2);
                        //TODO error can't have empty exponent
                    }
                } else if (isdigit(c)) {
                    if ((c == '0') && (first_num)) {
                        ragequit(str, 2);
                        //TODO toss error 0 na zacatku cisla (pozor na float)
                    }
                    append_to_string(string, c);
                } else {
                    ungetc(c, stdin);
                    break;
                }
            }
            //TODO check for zeroes at the end of a float?
            if (float_num) {
                create_token(token, TOKEN_DATA_DOUBLE, tok_list, string->str);
            } else {
                create_token(token, TOKEN_DATA_INT, tok_list, string->str);
            }

        } else if ((isalpha(c)) || (c == '_')) {
            restart_string(string);
            append_to_string(string, c);

            while (1) {
                if ((c == '_') || (isalnum(c))) {
                    append_to_string(string, c);

                    if (!strcmp(string->str, "do")) {
                        create_token(token, TOKEN_KEYWORD_DO, tok_list, string->str);
                        break;
                    } else if (strcmp(string->str, "else") == 0) {
                        create_token(token, TOKEN_KEYWORD_ELSE, tok_list, string->str);
                        break;
                    } else if (strcmp(string->str, "end") == 0) {
                        create_token(token, TOKEN_KEYWORD_END, tok_list, string->str);
                        break;
                    } else if (strcmp(string->str, "function") == 0) {
                        create_token(token, TOKEN_KEYWORD_FUNCTION, tok_list, string->str);
                        break;
                    } else if (strcmp(string->str, "global") == 0) {
                        create_token(token, TOKEN_KEYWORD_GLOBAL, tok_list, string->str);
                        break;
                    } else if (strcmp(string->str, "if") == 0) {
                        create_token(token, TOKEN_KEYWORD_IF, tok_list, string->str);
                        break;
                    } else if (strcmp(string->str, "integer") == 0) {
                        create_token(token, TOKEN_KEYWORD_INTEGER, tok_list, string->str);
                        break;
                    } else if (strcmp(string->str, "local") == 0) {
                        create_token(token, TOKEN_KEYWORD_LOCAL, tok_list, string->str);
                        break;
                    } else if (strcmp(string->str, "nil") == 0) {
                        create_token(token, TOKEN_KEYWORD_NIL, tok_list, string->str);
                        break;
                    } else if (strcmp(string->str, "number") == 0) {
                        create_token(token, TOKEN_KEYWORD_NUMBER, tok_list, string->str);
                        break;
                    } else if (strcmp(string->str, "require") == 0) {
                        create_token(token, TOKEN_KEYWORD_REQUIRE, tok_list, string->str);
                        break;
                    } else if (strcmp(string->str, "return") == 0) {
                        create_token(token, TOKEN_KEYWORD_RETURN, tok_list, string->str);
                        break;
                    } else if (strcmp(string->str, "string") == 0) {
                        create_token(token, TOKEN_KEYWORD_STRING, tok_list, string->str);
                        break;
                    } else if (strcmp(string->str, "then") == 0) {
                        create_token(token, TOKEN_KEYWORD_THEN, tok_list, string->str);
                        break;
                    } else if (strcmp(string->str, "while") == 0) {
                        create_token(token, TOKEN_KEYWORD_WHILE, tok_list, string->str);
                        break;
                    }
                } else {
                    create_token(token, TOKEN_ID, tok_list, string->str);
                    break;
                }
            }
            ungetc(c, stdin);
        } else {
            if (c == EOF) {
                return 0;
            } else {
                ragequit(str, 2);
                //TODO scanner error

            }
        }

    }
}
//TODO & and | what do :)