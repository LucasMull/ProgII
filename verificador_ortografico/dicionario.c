//GRR20197160 Lucas Müller

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "dicionario.h"

#define FILESIZE_COEFICIENT 10000

/* extrai informações do arquivo dicionário em um array de strings,
  com cada índice representando uma palavra e linha do dicionário */
static void
dictionary_parse(dictionary_st *dictionary, FILE *f_dictionary)
{
  long memblock=0; //conta qtd de blocos criados na memória
  char word[MAX_WORD_SIZE]; //armazena palavra por linha
  dictionary->word_list = NULL;

  /*lê arq linha por linha, cada linha uma palavra, e
    armazena as palavras em dictionary->word_list*/
  dictionary->num_word = 0;
  while (fgets(word, MAX_WORD_SIZE-1, f_dictionary) != NULL){
    /*se módulo da qtd de linhas com coeficiente especificado for
      zero, então aloca espaço na memória para prosseguir com leitura
      do arquivo*/
    if (0 == dictionary->num_word % FILESIZE_COEFICIENT){
      ++memblock;
      dictionary->word_list = realloc(dictionary->word_list,memblock*FILESIZE_COEFICIENT*sizeof(char*));
      assert(dictionary->word_list);
    }
    //aloca mem para palavra sem incluir newline
    dictionary->word_list[dictionary->num_word] = strndup(word,strlen(word)-1);
    assert(dictionary->word_list[dictionary->num_word]);

    ++dictionary->num_word;
  }
  assert(feof(f_dictionary));
}

/*retorna const char com strcasecmp para utilizar no qsort()*/
static int
cstrcmp(const void *a, const void *b) 
{ 
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcasecmp(*ia, *ib);
} 

void
dictionary_set(char dictionary_lang[], dictionary_st *dictionary)
{
  //anexa linguagem do dicionário escolhido ao path de dicionarios
  char dictionary_location[100] = "/usr/share/dict/";
  strcat(dictionary_location,dictionary_lang);

  FILE *f_dictionary = fopen(dictionary_location, "rb");
  if (NULL == f_dictionary){
    strcpy(dictionary_location,"dict/");
    strcat(dictionary_location,dictionary_lang);
    f_dictionary = fopen(dictionary_location, "rb");

    if (NULL == f_dictionary){
      f_dictionary = fopen(dictionary_lang, "rb");
    }
  }
  assert(f_dictionary);

  dictionary_parse(dictionary, f_dictionary); //aloca palavras do dicionario no struct dictionary

  //ordena palavras do dicionario para bsearch
  qsort(dictionary->word_list,dictionary->num_word,sizeof(char*),cstrcmp);

  fclose(f_dictionary);
}

/*libera espaço alocado para dictionary_st*/
void
dictionary_destroy(dictionary_st *dictionary){
  for (long i=0; i < dictionary->num_word; ++i)
    free(dictionary->word_list[i]);
  free(dictionary->word_list);
}

/*binary search com strcasecmp para comparar uma palavra-chave
  com uma array de strings pre-ordenadas*/
int
dictionary_bsearch(const dictionary_st *kdictionary, const char *kword)
{
  int top = kdictionary->num_word-1;
  int low = 0;
  int mid;

  int cmp;
  while (low <= top){
    mid = ((unsigned long)low + (unsigned long)top) >> 1;
    cmp = strcasecmp(kword, kdictionary->word_list[mid]);
    if (cmp == 0)
      return 0;
    if (cmp < 0)
      top = mid-1;
    else
      low = mid+1;
  }

  return cmp;
}

/*marca palavras com erros de escrita com colchetes*/
void
stream_misspell_check(const dictionary_st *kdictionary, FILE* out_stream)
{
  int c; //leitura de chars do stdin
  char word[MAX_WORD_SIZE]; //formação da palavra a ser verificada

  /* c lê o stdin um char por vez, com a condição de saída de chegar
  no fim do arquivo.
  enquanto c for um char alfabético, a palavra é formada na variável
  word, quando não for char alfabético, a palavra formada é então
  analisada para ver se existe ou não no dicionário*/
  int i=0;
  while (EOF != (c = getchar())){
    //se c for char alfabético então concatena no final da palvra
    //senão verifica existência da palavra formada no dicionário
    if (isalpha(c)){
      word[i] = c;
      ++i;
    } else {
      word[i] = '\0'; //concluir palavra formada
      //se palavra n for alpha, ou existir no dictionary imprime sem colchetes
      //senão imprime palavra com colchetes
      if ((0 == i) || (0 == dictionary_bsearch(kdictionary,word))){ 
        fputs(word,out_stream);
      } else {
        fprintf(out_stream,"[%s]",word);
      }

      i = 0; //reseta índice para formar nova palavra
      fputc(c,out_stream);//printa char não alpha encontrado
    }
  }
  assert(feof(stdin));
}

