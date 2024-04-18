Simulador de Memória Virtual em C
Este é um simulador de memória virtual implementado em C. Ele simula o comportamento de um sistema de gerenciamento de memória virtual, registrando o número de falhas de página e o número de páginas sujas escritas, com base em um algoritmo de substituição de página especificado (LRU ou NRU).


Para executar o programa, você deve fornecer os seguintes argumentos na linha de comando:

Algoritmo: O algoritmo de substituição de página a ser utilizado (LRU ou NRU).
Arquivo de Log: O arquivo contendo os endereços de memória e as operações de leitura/gravação a serem simuladas.
Tamanho da Página: O tamanho da página em kilobytes (KB), suportando 8 KB ou 16 KB.
Tamanho da Memória: O tamanho total da memória em megabytes (MB), entre 1 MB e 4 MB.

Exemplo de execução:
./simulador LRU compressor.log 16 4
