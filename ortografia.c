#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <assert.h>

#define MAX_WORD_SIZE 50

typedef struct {
  char **wlist; //word list (array de strings)
  long wcount; //word count (qtd de palavras)
} dict_t;

long
get_fileln(FILE *file)
{
  int ln=0;

  int c;
  while ((c=fgetc(file)) != EOF){ //lê até fim do arqv
    if (c=='\n') //conta uma linha
      ++ln;
  } 
  assert(feof(file)); //checa se leu arq corretamente
  fseek(file,0,SEEK_SET); //volta pro começo do file

  return ln;
}

void
set_dict(char dict_location[], dict_t *dict)
{
  FILE *f_dict=fopen(dict_location, "rb");
  assert(f_dict); //checa se arq foi lido

  char word[MAX_WORD_SIZE]; //armazena palavra por linha
  
  dict->wcount=get_fileln(f_dict); //retorna qtd de ln do arq 
  dict->wlist=malloc(dict->wcount*sizeof(char*));
  assert(dict->wlist); //verifica mem alocada corretamente 

  /*lê arq linha por linha, cada linha uma palavra, e
    armazena as palavras em dict->wlist*/
  long i=0;
  while (fgets(word, MAX_WORD_SIZE-1, f_dict) != NULL){
    //aloca mem para palavra sem incluir chars '\n'
    dict->wlist[i] = strndup(word,strlen(word)-1);
    ++i;
  }

  assert(i == dict->wcount);

  //checa se terminou a leitura corretamente
  assert(feof(f_dict));

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
dict_bsearch(dict_t *dict, char *word)
{
  int top=dict->wcount-1;
  int low=0;
  int mid;

  int cmp;
  while (low <= top){
    mid = ((unsigned long)low + (unsigned long)top) >> 1;
    cmp = strcmp(word, dict->wlist[mid]);
    if (cmp == 0)
      return 0;
    if (cmp < 0)
      top = mid-1;
    else
      low = mid+1;
  }

  return cmp;
}

/*retorna const char com strcmp para utilizar no qsort()*/
int
cstring_cmp(const void *a, const void *b) 
{ 
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
} 

int main(void)
{
  char *locale=setlocale(LC_CTYPE, "pt_BR.iso88591");
  assert(locale); //verifica se locale foi setado

  /*insere arquivo dicionario na memoria como um
    array de strings */
  dict_t dict;
  set_dict("dict/brazilian", &dict);

  //ordena palavras do dicionario para bsearch*/
  qsort(dict.wlist,dict.wcount,sizeof(char*),cstring_cmp);

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
      if ((i==0) || (dict_bsearch(&dict,word) == 0))
        fputs(word,stdout); //imprime palavra s/colchetes
      else //se não estiver no dict
        fprintf(stdout,"[%s]",word); //imprime com colchetes
      fputc(c,stdout);
      i = 0;
    }
  }

  destroy_dict(&dict);

  return EXIT_SUCCESS;
}
