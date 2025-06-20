#include "expressao.h"
#include <stdio.h>
#include <string.h>

void testarExpressao(const char* posFixa, const char* inFixaEsperada, float valorEsperado) {
    printf("Pos-fixa: %s\n\n", posFixa);

    char strPosFixa[512];
    strcpy(strPosFixa, posFixa);

    float valor = getValorPosFixa(strPosFixa);
    printf("Valor calculado: %.2f | Valor esperado: %.2f\n", valor, valorEsperado);

    strcpy(strPosFixa, posFixa);
    char* inFixa = getFormaInFixa(strPosFixa);
    printf("Forma infixa: %s | Forma infixa esperada: %s\n\n\n", inFixa, inFixaEsperada);
    
}

void testarConversaoInFixa(const char* inFixa, const char* posFixaEsperada) {
    printf("Infixa: %s\n", inFixa);

    char strInFixa[512];
    strcpy(strInFixa, inFixa);

    char* posFixa = getFormaPosFixa(strInFixa);
    printf("Forma pos-fixa: %s\nForma pos-fixa esperada: %s\n\n", posFixa, posFixaEsperada);
}

int main() {
    printf("\n------------ Pos-fixada para infixada + calculo da expressao ------------\n\n");
    testarExpressao("3 4 + 5 *", "(3 + 4) * 5", 35);
    testarExpressao("7 2 * 4 +", "7 * 2 + 4", 18);
    testarExpressao("8 5 2 4 + * +", "8 + (5 * (2 + 4))", 38);
    testarExpressao("6 2 / 3 + 4 *", "(6 / 2 + 3) * 4", 24);
    testarExpressao("9 5 2 8 * 4 + * +", "9 + (5 * (2 + 8 * 4))", 109);
    testarExpressao("2 3 + log 5 /", "log(2 + 3) / 5", 0.14f);
    testarExpressao("10 log 3 ^ 2 +", "(log10)^3 + 2", 3);
    testarExpressao("45 60 + 30 cos *", "(45 + 60) * cos(30)", 90.93f);
    testarExpressao("0.5 45 sen 2 ^ +", "sen(45)^2 + 0.5", 1);

    printf("\n------------ Infixada para pos-fixada------------\n\n");
    testarConversaoInFixa("(3 + 4) * 5", "3 4 + 5 *");
    testarConversaoInFixa("7 * 2 + 4", "7 2 * 4 +");
    testarConversaoInFixa("8 + (5 * (2 + 4))", "8 5 2 4 + * +");
    testarConversaoInFixa("(6 / 2 + 3) * 4", "6 2 / 3 + 4 *");
    testarConversaoInFixa("9 + (5 * (2 + 8 * 4))", "9 5 2 8 * 4 + * +");
    testarConversaoInFixa("log(2 + 3) / 5", "2 3 + log 5 /");
    testarConversaoInFixa("(log10)^3 + 2", "10 log 3 ^ 2 +");
    testarConversaoInFixa("(45 + 60) * cos(30)", "45 60 + 30 cos *");
    testarConversaoInFixa("sen(45) ^2 + 0,5", "0.5 45 sen 2 ^ +");

    return 0;
}
