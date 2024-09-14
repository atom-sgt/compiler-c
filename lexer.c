#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

Token *createToken(TokenType type, const char *value) {
    Token *token = (Token *)malloc(sizeof(Token));
    token->type = type;
    token->value = strdup(value);
    return token;
}

void freeToken(Token *token) {
    free(token->value);
    free(token);
}

void printToken(const Token *token) {
    const char *typeStr;
    switch (token->type) {
        case TOKEN_IDENTIFIER: typeStr = "IDENTIFIER"; break;
        case TOKEN_NUMBER: typeStr = "NUMBER"; break;
        case TOKEN_OPERATOR: typeStr = "OPERATOR"; break;
        case TOKEN_PUNCTUATION: typeStr = "PUNCTUATION"; break;
        default: typeStr = "UNKNOWN"; break;
    }
    printf("Token: Type=%s, Value=%s\n", typeStr, token->value);
}

int isPunctuation(char c) {
    return c == ';' 
		|| c == ','
		|| c == '('
		|| c == ')'
		|| c == '{'
		|| c == '}';
}

int isOperator(char c) {
	return c == '+'
		|| c == '-'
		|| c == '*'
		|| c == '/'
		|| c == '='
		|| c == '!'; 
}

int isCompoundOperator(char c1, char c2) {
	return (c1 == '+' && c2 == '+')
		|| (c1 == '-' && c2 == '-')
		|| (c1 == '=' && c2 == '=')
		|| (c1 == '!' && c2 == '=');
}

void lex(const char *input) {
    const char *p = input;
	while (*p) {
        // Skip whitespace
        if (isspace(*p)) {
            p++;
        }

        // Identify numbers
        if (isdigit(*p)) {
            const char *start = p;
            while (isdigit(*p)) p++;

            char *value = strndup(start, p - start);
            Token *token = createToken(TOKEN_NUMBER, value);
            printToken(token);
            freeToken(token);
            free(value);

            continue;
        }

        // Identify identifiers
        if (isalpha(*p)) {
            const char *start = p;
            while (isalnum(*p) || *p == '_') p++;

            char *value = strndup(start, p - start);
            Token *token = createToken(TOKEN_IDENTIFIER, value);
            printToken(token);
            freeToken(token);
            free(value);

            continue;
        }

        // Identify operators
        if (isOperator(*p)) {
            const char *start = p;
            p++;
			if(isCompoundOperator(*start, *p)) {
                p++;
            }

            char *value = strndup(start, p - start);
            Token *token = createToken(TOKEN_OPERATOR, value);
            printToken(token);
            freeToken(token);
            free(value);

            continue;
        }

        // Identify punctuation
        if (isPunctuation(*p)) {
            char value[2] = {*p, '\0'};
            Token *token = createToken(TOKEN_PUNCTUATION, value);
			p++;
            printToken(token);
            freeToken(token);

            continue;
        }

        // Unknown token
        char value[2] = {*p, '\0'};
        Token *token = createToken(TOKEN_UNKNOWN, value);
		p++;
        printToken(token);
        freeToken(token);
    }
}

int main() {
    const char *input = "int main() { int a = 42; a++; }";
    lex(input);
    return 0;
}

