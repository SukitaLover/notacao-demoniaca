#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pilha.hpp"

bool estaNoConjuntoChar(char conjunto[], int tamanho, char elemento) {
    for (int i = 0; i < tamanho; i++) {
        if (conjunto[i] == elemento) {
            return true; // Elemento encontrado
        }
    }
    return false; // Elemento não encontrado
}

// Função para obter a prioridade do operador
int prioridade(char operador) {
    switch (operador) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        default: return 0; // Parênteses ou caracteres inválidos
    }
}

// Resolução parte 4 projeto
int avaliarExpressaoPosfixa(char *posfixa, int valores[]) {
    Stack *pilha = CriaStack(50);
    
    for (int i = 0; posfixa[i] != '\0'; i++) {
        char c = posfixa[i];
        
        if (c >= 'A' && c <= 'Z') {
            //Empilha o valor da variavel correspondente
            push(pilha, valores[c - 'A']);
        } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            // Desempilha os dois últimos valores
            int b = pop(pilha);
            int a = pop(pilha);
            int resultado;
            
            //switch das operações do projeto
            switch (c) {
                case '+': resultado = a + b; break;
                case '-': resultado = a - b; break;
                case '*': resultado = a * b; break;
                case '/': resultado = (b != 0) ? a / b : 0; break;
                case '^': {
                    resultado = 1;
                    for (int j = 0; j < b; j++) resultado *= a;
                    break;
                }
            }
            
            // Empilha o resultado
            push(pilha, resultado);
        }
    }
    
    int resultadoFinal = pop(pilha);
    DestroiStack(pilha);
    return resultadoFinal;
}

int main() {
    char *texto = (char *)malloc(100 * sizeof(char));
    if (!texto) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    printf("Digite a Expressão Infixa que deseja operar: ");
    scanf("%s", texto);
    printf("Expressão digitada: %s\n", texto);

    char *saida = (char *)malloc(100 * sizeof(char));
    if (!saida) {
        printf("Erro ao alocar memória!\n");
        free(texto);
        return 1;
    }

    StackChar *pilhaop = CriaStackChar(50);
    int j = 0; 

    for (int i = 0; texto[i] != '\0' && texto[i] != '\n'; i++) {
        char c = texto[i];

        if (c >= 'A' && c <= 'Z') {
            saida[j++] = c; 
        } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            while (!isEmptyChar(pilhaop) && prioridade(topoChar(pilhaop)) >= prioridade(c)) {
                saida[j++] = popChar(pilhaop);
            }
            pushChar(pilhaop, c);
        } else if (c == '(') {
            pushChar(pilhaop, c);
        } else if (c == ')') {
            while (!isEmptyChar(pilhaop) && topoChar(pilhaop) != '(') {
                saida[j++] = popChar(pilhaop);
            }
            popChar(pilhaop);
        } else if (c != ' ') { 
            printf("Operação ou nome de variável inválidos!\n");
            return 0;
        }
    }

    while (!isEmptyChar(pilhaop)) {
        saida[j++] = popChar(pilhaop);
    }

    saida[j] = '\0'; 
    printf("Expressão Pós-Fixa: %s\n", saida);

    int valores[26] = {0};
    bool valores_preenchidos[26] = {false};
    
    for (int i = 0; saida[i] != '\0'; i++) {
        char c = saida[i];
        if (c >= 'A' && c <= 'Z') {
            if (!valores_preenchidos[c - 'A']) {
                printf("Digite um valor para %c: ", c);
                scanf("%d", &valores[c - 'A']);
                valores_preenchidos[c - 'A'] = true;
            }
        }    
    }

    printf("Os valores fornecidos foram:\n");
    for (int i = 0; i < 26; i++) {
        if (valores_preenchidos[i]) {
            printf("%c: %d\n", 'A' + i, valores[i]);
        }
    }
    
    int resultado = avaliarExpressaoPosfixa(saida, valores);
    printf("Resultado da expressão: %d\n", resultado);
    
    free(texto);
    free(saida);
    DestroiStackChar(pilhaop);
    return 0;
}
