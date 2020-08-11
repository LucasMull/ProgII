#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "dicionario.h"

/* converte uppercase para lowercase */
char*
word_tolower(const char *word){
  char *ptr_word = (char*)word;
  while (isalpha(*ptr_word)){
    *ptr_word = tolower(*ptr_word);
    ++ptr_word;
  }

  return (char*)word;
}

static void
parse_dict(dict_t *dict, FILE *f_dict)
{
  dict->wcount=0;

  int c;
  while ((c=fgetc(f_dict)) != EOF){ //lê até fim do arqv
    if (c=='\n') //conta uma linha
      ++dict->wcount;
  } 
  assert(feof(f_dict)); //checa se leu arq corretamente
  fseek(f_dict,0,SEEK_SET); //volta pro começo do f_dict

  dict->wlist=malloc(dict->wcount*sizeof(char*));
  assert(dict->wlist); //verifica mem alocada corretamente 

  /*lê arq linha por linha, cada linha uma palavra, e
    armazena as palavras em dict->wlist*/
  long i=0;
  char word[MAX_WORD_SIZE]; //armazena palavra por linha
  while (fgets(word, MAX_WORD_SIZE-1, f_dict) != NULL){
    word_tolower(word); //converte chars maiusc para minusc
    //aloca mem para palavra sem incluir chars '\n'
    dict->wlist[i] = strndup(word,strlen(word)-1);
    ++i;
  }
  assert(i == dict->wcount);
  //checa se terminou a leitura corretamente
  assert(feof(f_dict));
}

/*retorna const char com strcmp para utilizar no qsort()*/
static int
cstring_cmp(const void *a, const void *b) 
{ 
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
} 

void
set_dict(char dict_lang[], dict_t *dict)
{
  //anexa linguagem do dicionário escolhido ao path de dicionarios
  char dict_location[100] = "/usr/share/dict/";
  strcat(dict_location,dict_lang);

  FILE *f_dict=fopen(dict_location, "rb");
  assert(f_dict); //checa se arq foi lido (ou existe);

  parse_dict(dict, f_dict); //configura var dicionario

  //ordena palavras do dicionario para bsearch*/
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

/*binary search com strcmp para comparar uma palavra-chave
  com uma array de strings pre-ordenadas*/
int
dict_bsearch(const dict_t *dict, const char *word)
{
  char lower_word[MAX_WORD_SIZE];
  strncpy(lower_word,word,MAX_WORD_SIZE-1);
  word_tolower(lower_word);

  int top=dict->wcount-1;
  int low=0;
  int mid;

  int cmp;
  while (low <= top){
    mid = ((unsigned long)low + (unsigned long)top) >> 1;
    cmp = strcmp(lower_word, dict->wlist[mid]);
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
  char word[MAX_WORD_SIZE]; //forma palavra a ser pesquisada

  int i=0;
  while ((c=getchar()) != EOF){ //recolhe char até fim do arq 
    if (isalpha(c)){ //se char for alpha
      word[i] = c; //forma palavra
      ++i; //e incremente posição para proximo char
    } else { //char não é alpha
      word[i] = '\0'; //concluir palavra formada
      //se palavra n for alpha, ou existir no dict
      if ((i==0) || (dict_bsearch(dict,word) == 0))
        fputs(word,out_stream); //imprime palavra s/colchetes
      else //se não estiver no dict
        fprintf(out_stream,"[%s]",word); //imprime com colchetes
      fputc(c,out_stream);
      i = 0;
    }
  }
}

