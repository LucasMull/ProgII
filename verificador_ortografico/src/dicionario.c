#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "dicionario.h"

#define FILESIZE_COEFICIENT 131072

/* extrai informações do arquivo dicionário em um array de strings,
  com cada índice representando uma palavra e linha do dicionário */
static void
parse_dict(dict_t *dict, FILE *f_dict)
{
  long memblock=0; //conta qtd de blocos criados na memória
  char word[MAX_WORD_SIZE]; //armazena palavra por linha
  dict->wlist = NULL;

  /*lê arq linha por linha, cada linha uma palavra, e
    armazena as palavras em dict->wlist*/
  dict->wcount = 0;
  while (fgets(word, MAX_WORD_SIZE-1, f_dict) != NULL){
    /*se qtd de linhas for multiplo do coeficiente especificado
      então aloca espaço na memória para prosseguir com leitura
      do arquivo*/
    if (dict->wcount % FILESIZE_COEFICIENT == 0){
      ++memblock;
      dict->wlist = realloc(dict->wlist,memblock*FILESIZE_COEFICIENT*sizeof(char*));
      assert(dict->wlist);
    }
    //aloca mem para palavra sem incluir newline
    dict->wlist[dict->wcount] = strndup(word,strlen(word)-1);
    assert(dict->wlist[dict->wcount]);

    ++dict->wcount;
  }
  assert(feof(f_dict));
}

/*retorna const char com strcasecmp para utilizar no qsort()*/
static int
cstring_cmp(const void *a, const void *b) 
{ 
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcasecmp(*ia, *ib);
} 

void
set_dict(char dict_lang[], dict_t *dict)
{
  //anexa linguagem do dicionário escolhido ao path de dicionarios
  char dict_location[100] = "/usr/share/dict/";
  strcat(dict_location,dict_lang);

  FILE *f_dict=fopen(dict_location, "rb");
  assert(f_dict);

  parse_dict(dict, f_dict); //set membros do struct dict

  //ordena palavras do dicionario para bsearch
  qsort(dict->wlist,dict->wcount,sizeof(char*),cstring_cmp);

  fclose(f_dict);
}

/*libera espaço alocado para dict_t*/
void
destroy_dict(dict_t *dict){
  for (long i=0; i < dict->wcount; ++i)
    free(dict->wlist[i]);
  free(dict->wlist);
}

/*binary search com strcasecmp para comparar uma palavra-chave
  com uma array de strings pre-ordenadas*/
int
dict_bsearch(const dict_t *dict, const char *word)
{
  int top=dict->wcount-1;
  int low=0;
  int mid;

  int cmp;
  while (low <= top){
    mid = ((unsigned long)low + (unsigned long)top) >> 1;
    cmp = strcasecmp(word, dict->wlist[mid]);
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
mispelling_mark(const dict_t *dict, FILE* out_stream)
{
  int c; //leitura de chars do stdin
  char word[MAX_WORD_SIZE]; //formação da palavra a ser verificada

  /* c lê o stdin um char por vez, com a condição de saída de chegar
  no fim do arquivo (EOF).
  enquanto c for um char alfabético, a palavra é formada na variável
  word, quando não for char alfabético, a palavra formada é então
  analisada para ver se existe ou não no dicionário*/
  int i=0;
  while ((c=getchar()) != EOF){
    //se c for alfabético então concatena no final da palvra
    //senão verifica existência da palavra formada no dicionário
    if (isalpha(c)){
      word[i] = c;
      ++i;
    } else {
      word[i] = '\0'; //concluir palavra formada
      //se palavra n for alpha, ou existir no dict imprime sem colchetes
      //senão imprime palavra com colchetes
      if ((i == 0) || (dict_bsearch(dict,word) == 0)){ 
        fputs(word,out_stream);
      } else {
        fprintf(out_stream,"[%s]",word);
      }

      i = 0; //reseta índice para formar nova palavra
      fputc(c,out_stream);//printa char não alpha encontrado
    }
  }
}

