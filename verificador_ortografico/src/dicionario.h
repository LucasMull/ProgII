#define MAX_WORD_SIZE 50

typedef struct {
  char **word_list; //(array de strings)
  long num_word; //(qtd de palavras)
} dictionary_st;

/*seta valores dos membros do struct dictionary_t*/
void dictionary_set(char dictionary_lang[], dictionary_st *dictionary);

/*libera espaço alocado para dictionary_t*/
void dictionary_destroy(dictionary_st *dictionary);

/*binary search com strcmp para comparar uma palavra-chave
  com uma array de strings pre-ordenadas*/
int dictionary_bsearch(const dictionary_st *kdictionary, const char *kword);

/*marca palavras com erros de escrita com colchetes
  dictionary_t* recebe o dicionário a ser utilizado nas
  comparações, out_stream indica a stream em que o
  texto com marcações será escrito*/
void stream_misspell_check(const dictionary_st *kdictionary, FILE* out_stream);
