#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expressao.h"
#include <ctype.h>

#define MAX 512

typedef struct {
    float itens[MAX];
    int topo;
} PilhaFloat;

void initPilhaFloat(PilhaFloat* p) {
    p->topo = -1;
}

int vaziaFloat(PilhaFloat* p) {
    return p->topo == -1;
}

void pushFloat(PilhaFloat* p, float val) {
    p->itens[++p->topo] = val;
}

float popFloat(PilhaFloat* p) {
    return p->itens[p->topo--];
}

typedef struct {
    char* itens[MAX];
    int topo;
} PilhaStr;

void initPilhaStr(PilhaStr* p) {
    p->topo = -1;
}

void pushStr(PilhaStr* p, const char* str) {
    p->itens[++p->topo] = strdup(str);
}

char* popStr(PilhaStr* p) {
    return p->itens[p->topo--];
}

float grausParaRadianos(float graus) {
    return graus * M_PI / 180.0;
}

int isFuncao(const char* s) {
    return (
        strcmp(s, "sen") == 0 || strcmp(s, "cos") == 0 || 
        strcmp(s, "tg") == 0 || strcmp(s, "log") == 0 || 
        strcmp(s, "raiz") == 0
    );
}

int isOperador(const char* s) {
    return (
        strcmp(s, "+") == 0 || strcmp(s, "-") == 0 || strcmp(s, "*") == 0 || 
        strcmp(s, "/") == 0 || strcmp(s, "^") == 0 || strcmp(s, "%") == 0
    );
}

float getValorPosFixa(char* StrPosFixa) {
    PilhaFloat pilha;
    initPilhaFloat(&pilha);

    char* token = strtok(StrPosFixa, " ");
    while (token) {
        if (isOperador(token)) {
            float b = popFloat(&pilha);
            float a = popFloat(&pilha);
            if (strcmp(token, "+") == 0) pushFloat(&pilha, a + b);
            else if (strcmp(token, "-") == 0) pushFloat(&pilha, a - b);
            else if (strcmp(token, "*") == 0) pushFloat(&pilha, a * b);
            else if (strcmp(token, "/") == 0) pushFloat(&pilha, a / b);
            else if (strcmp(token, "^") == 0) pushFloat(&pilha, powf(a, b));
            else if (strcmp(token, "%") == 0) pushFloat(&pilha, fmodf(a, b));
        } else if (isFuncao(token)) {
            float a = popFloat(&pilha);
            if (strcmp(token, "sen") == 0) pushFloat(&pilha, sinf(grausParaRadianos(a)));
            else if (strcmp(token, "cos") == 0) pushFloat(&pilha, cosf(grausParaRadianos(a)));
            else if (strcmp(token, "tg") == 0) pushFloat(&pilha, tanf(grausParaRadianos(a)));
            else if (strcmp(token, "log") == 0) pushFloat(&pilha, log10f(a));
            else if (strcmp(token, "raiz") == 0) pushFloat(&pilha, sqrtf(a));
        } else {
            pushFloat(&pilha, atof(token));
        }
        token = strtok(NULL, " ");
    }
    return vaziaFloat(&pilha) ? 0.0 : popFloat(&pilha);
}

char* getFormaInFixa(char* Str) {
    PilhaStr pilha;
    initPilhaStr(&pilha);

    char* token = strtok(Str, " ");
    while (token) {
        if (isOperador(token)) {
            char* b = popStr(&pilha);
            char* a = popStr(&pilha);
            char buffer[MAX];
            snprintf(buffer, MAX, "(%s %s %s)", a, token, b);
            pushStr(&pilha, buffer);
            free(a); free(b);
        } else if (isFuncao(token)) {
            char* a = popStr(&pilha);
            char buffer[MAX];
            snprintf(buffer, MAX, "%s(%s)", token, a);
            pushStr(&pilha, buffer);
            free(a);
        } else {
            pushStr(&pilha, token);
        }
        token = strtok(NULL, " ");
    }

    return popStr(&pilha);
}

int ehOperador(char c) {
    return strchr("+-*/%^", c) != NULL;
}

int prioridade(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': case '%': return 2;
        case '^': return 3;
        default: return 0;
    }
}

char* getFormaPosFixa(char* Str) {
    static char saida[MAX];
    saida[0] = '\0';

    char* stack[MAX];
    int topo = -1;

    char token[MAX];
    int i = 0, j = 0;

    while (Str[i] != '\0') {
        char c = Str[i];

        if (isdigit(c) || c == '.') {
            token[j++] = c;
        } else {
            if (j > 0) {
                token[j] = '\0';
                strcat(saida, token);
                strcat(saida, " ");
                j = 0;
            }

            if (isalpha(c)) {
                j = 0;
                while (isalpha(Str[i])) {
                    token[j++] = Str[i++];
                }
                token[j] = '\0';

                stack[++topo] = strdup(token);
                j = 0;
                continue;
            }

            if (c == '(') {
                stack[++topo] = strdup("(");
            } else if (c == ')') {
                while (topo >= 0 && strcmp(stack[topo], "(") != 0) {
                    strcat(saida, stack[topo--]);
                    strcat(saida, " ");
                }
                if (topo >= 0) free(stack[topo--]);

                if (topo >= 0 && isFuncao(stack[topo])) {
                    strcat(saida, stack[topo--]);
                    strcat(saida, " ");
                }
            } else if (ehOperador(c)) {
                while (topo >= 0 && ehOperador(stack[topo][0]) &&
                       prioridade(stack[topo][0]) >= prioridade(c)) {
                    strcat(saida, stack[topo--]);
                    strcat(saida, " ");
                }
                char* op = malloc(2);
                op[0] = c;
                op[1] = '\0';
                stack[++topo] = op;
            }
        }

        i++;
    }

    if (j > 0) {
        token[j] = '\0';
        strcat(saida, token);
        strcat(saida, " ");
    }

    while (topo >= 0) {
        strcat(saida, stack[topo--]);
        strcat(saida, " ");
    }

    return saida;
}

float getValorInFixa(char* StrInFixa) {
    char copia[MAX];
    strcpy(copia, StrInFixa);
    char* posfixa = getFormaPosFixa(copia);
    return getValorPosFixa(posfixa);
}
