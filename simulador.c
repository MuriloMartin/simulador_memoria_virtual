#include <stdio.h>
#include <stdlib.h>

#define MAX_MEMORY_SIZE 4 * 1024 * 1024 // 4MB
#define MAX_PAGE_SIZE 16 * 1024 // 16KB

void verifica_limites(int page_size, int total_memory) {
    if (page_size != 8192 && page_size != 16384) {
        fprintf(stderr, "Tamanho de página não suportado. Use 8 ou 16 KB.\n");
        exit(EXIT_FAILURE);
    }

    if (total_memory < 1024 * 1024 || total_memory > MAX_MEMORY_SIZE) {
        fprintf(stderr, "Tamanho de memória não suportado. Use entre 1 MB e 4 MB.\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Uso: %s <algoritmo> <arq.log> <tamanho_pagina> <tamanho_memoria>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Argumentos de linha de comando
    char *algorithm = argv[1];
    char *log_file = argv[2];
    int page_size = atoi(argv[3]) * 1024; // Convertendo para bytes
    int total_memory = atoi(argv[4]) * 1024 * 1024; // Convertendo para bytes

    // Verificar limites
    verifica_limites(page_size, total_memory);
    

    // Imprime os parametros passados pela linha de comando
    printf("Algoritmo: %s\n", algorithm);
    printf("Arquivo de log: %s\n", log_file);
    printf("Tamanho de página: %d KB\n", page_size / 1024);
    printf("Tamanho de memória: %d MB\n", total_memory / (1024 * 1024));

   

 

   


    return 0;
}