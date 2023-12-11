#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

#define MAX_MEMORY_SIZE 4 * 1024 * 1024 // 4MB
#define MAX_PAGE_SIZE 16 * 1024         // 16KB

typedef struct
{
    int page_index;
    bool referenced;
    bool modified;
    int lastAcess;

} FrameInfo;

typedef struct
{
    FrameInfo *frames;
    int num_frames;
    int page_faults;
    int dirty_pages_written;
} PageTable;

void verifica_limites(int page_size, int total_memory)
{
    if (page_size != 8192 && page_size != 16384)
    {
        fprintf(stderr, "Tamanho de página não suportado. Use 8 ou 16 KB.\n");
        exit(EXIT_FAILURE);
    }

    if (total_memory < 1024 * 1024 || total_memory > MAX_MEMORY_SIZE)
    {
        fprintf(stderr, "Tamanho de memória não suportado. Use entre 1 MB e 4 MB.\n");
        exit(EXIT_FAILURE);
    }
}

void prtint_parameters(char *algorithm, char *log_file, int page_size, int total_memory, int shift)
{
    printf("Algoritmo: %s\n", algorithm);
    printf("Arquivo de log: %s\n", log_file);
    printf("Tamanho de página: %d KB\n", page_size / 1024);
    printf("Tamanho de memória: %d MB\n", total_memory / 1024 / 1024);
    printf("Deslocamento: %d MB\n", shift);
}

void verifica_arquivo(FILE *file)
{
    if (file == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo de log.\n");
        exit(EXIT_FAILURE);
    }
}

int getShift(int page_size)
{
    int shift;
    double log2_numero = log2(page_size);
    shift = (int)round(log2_numero);
    return shift;
}

int getIndex(unsigned int addr, int shift)
{
    unsigned int index = addr >> shift;

    return index;
}

void initializePageTable(PageTable *page_table, int num_frames)
{
    page_table->frames = malloc(sizeof(FrameInfo) * num_frames);
    page_table->num_frames = num_frames;

    for (int i = 0; i < num_frames; i++)
    {
        page_table->frames[i].page_index = -1;
        page_table->frames[i].referenced = false;
        page_table->frames[i].modified = false;
        page_table->frames[i].lastAcess = 2;
    }

    page_table->page_faults = 0;
    page_table->dirty_pages_written = 0;
}

void freePageTable(PageTable *page_table)
{
    free(page_table->frames);
}

bool validPageCheck(PageTable *page_table, int page_index, char operation, time_t time)
{

    for (int i = 0; i < page_table->num_frames; i++)
    {
        if (page_table->frames[i].page_index == page_index)
        {
            page_table->frames[i].referenced = true;
            page_table->frames[i].lastAcess = time;

            if (operation == 'W')
            {
                page_table->frames[i].modified = true;
            }

            return true;
        }
    }
    return false;
}

int choosePageToReplace_LRU(PageTable *page_table)
{
    int chosen_index = 0;

    int oldest_access = page_table->frames[0].lastAcess;

    for (int i = 0; i < page_table->num_frames; i++)
    {
        if (page_table->frames[i].lastAcess <= oldest_access)
        {
            oldest_access = page_table->frames[i].lastAcess;
            chosen_index = i;
        }
    }
    return chosen_index;
}

void handlePageAccess(PageTable *page_table, unsigned int logical_address, char operation, int shift, char *algorithm, int* time)
{
    *time += 1;
    unsigned int page_index = getIndex(logical_address, shift);
    bool page_found = validPageCheck(page_table, page_index, operation, *time);

    if (!page_found)
    {
        page_table->page_faults++;
        for (int i = 0; i < page_table->num_frames; i++)
        {
            if (page_table->frames[i].page_index == -1)
            {
                page_table->frames[i].page_index = page_index;
                page_table->frames[i].referenced = true;
                page_table->frames[i].lastAcess = *time;
                page_table->frames[i].modified = (operation == 'W');
                page_found = true;
                break;
            }
        }

        if (!page_found)
        {

            int index_replace;

            if (strcmp(algorithm, "NRU") == 0)
            {
                // printf("Algoritmo NRU será implementado agora:");
                //   choosePageToReplace_NRU(page_table, logical_address, operation, shift);
            }
            else if (strcmp(algorithm, "LRU") == 0)
            {

                index_replace = choosePageToReplace_LRU(page_table);
                printf("Página substituída: %x\n", page_table->frames[index_replace].page_index);
            }
            else
            {
                fprintf(stderr, "Algoritmo não suportado: %s\n", algorithm);
                exit(EXIT_FAILURE);
            }
            page_table->dirty_pages_written += page_table->frames[index_replace].modified;

            page_table->frames[index_replace].page_index = page_index;
            page_table->frames[index_replace].referenced = true;
            page_table->frames[index_replace].modified = (operation == 'W');
            page_table->frames[index_replace].lastAcess = *time;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Uso: %s <algoritmo> <arq.log> <tamanho_pagina> <tamanho_memoria>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // Argumentos de linha de comando
    char *algorithm = argv[1];
    char *log_file = argv[2];
    int page_size = atoi(argv[3]); //* 1024; // Convertendo para bytes
    int total_memory = atoi(argv[4]); // * 1024 * 1024; // Convertendo para bytes
    FILE **files = malloc(sizeof(FILE *) * 4);
    int shift = getShift(page_size);
    FILE *file = fopen(log_file, "r");
    int num_frames = total_memory / page_size;
    unsigned int addr;
    char rw;
    PageTable page_table;
    int time = 0;

    //verifica_limites(page_size, total_memory);
    prtint_parameters(algorithm, log_file, page_size, total_memory, shift);
    verifica_arquivo(file);
    initializePageTable(&page_table, num_frames);

    while (fscanf(file, "%x %c", &addr, &rw) == 2)
    {
        //  printf("Address: 0x%x, Read/Write: %c, Page index: 0x%x - %u\n", addr, rw, getIndex(addr, shift), getIndex(addr, shift));
        handlePageAccess(&page_table, addr, rw, shift, algorithm, &time);
    }
    printf("Número de falhas de página: %d\n", page_table.page_faults);
    printf("Número de páginas sujas escritas: %d\n", page_table.dirty_pages_written);

    freePageTable(&page_table);
    fclose(file);

    return 0;
}