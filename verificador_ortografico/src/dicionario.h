#define MAX_WORD_SIZE 50

typedef struct {
  char **wlist; //word list (array de strings)
  long wcount; //word count (qtd de palavras)
} dict_t;

/* converte uppercase para lowercase,
  retorna ponteiro para palavra convertida*/
char* word_tolower(const char *word);

/*seta valores dos membros do struct dict_t*/
void set_dict(char dict_lang[], dict_t *dict);

/*libera espaço alocado para dict_t*/
void destroy_dict(dict_t *dict);

/*binary search com strcmp para comparar uma palavra-chave
  com uma array de strings pre-ordenadas*/
int dict_bsearch(const dict_t *dict, const char *word);

/*marca palavras com erros de escrita com colchetes
  dict_t* recebe o dicionário a ser utilizado nas
  comparações, out_stream indica a stream em que o
  texto com marcações será escrito*/
void mispelling_mark(const dict_t *dict, FILE* out_stream);
