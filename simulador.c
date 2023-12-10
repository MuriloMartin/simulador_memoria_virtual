#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

void prtint_parameters(char *algorithm, char *log_file, int page_size, int total_memory, int shift) {
    printf("Algoritmo: %s\n", algorithm);
    printf("Arquivo de log: %s\n", log_file);
    printf("Tamanho de página: %d KB\n", page_size / 1024);
    printf("Tamanho de memória: %d MB\n", total_memory / 1024 / 1024);
    printf("Deslocamento: %d MB\n", shift);
}

void verifica_arquivo(FILE *file) {
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de log.\n");
        exit(EXIT_FAILURE);
    }
}

int getShift(int page_size){
    int shift;
    double log2_numero = log2(page_size);
    shift = (int)round(log2_numero);
    return shift;
}
int getIndex(unsigned int addr, int shift){
    unsigned int index = addr >> shift;

    return index;
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
    FILE **files = malloc(sizeof(FILE *) * 4);
    verifica_limites(page_size, total_memory);
    int shift = getShift(page_size);
    prtint_parameters(algorithm, log_file, page_size, total_memory, shift);
    FILE *file = fopen(log_file, "r");
    verifica_arquivo(file);

    unsigned int addr;
    char rw;

    while (fscanf(file, "%x %c", &addr, &rw) == 2) {
        printf("Address: 0x%x, Read/Write: %c, Page index: 0x%x - %u\n", addr, rw, getIndex(addr, shift), getIndex(addr, shift));
    }

    fclose(file);

    return 0;
}