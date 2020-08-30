//GRR20197160 Lucas Müller

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <assert.h>

#include "src/dicionario.h"

int main(void)
{
  char *locale = setlocale(LC_CTYPE, "pt_BR.iso88591");
  assert(NULL != locale); //verifica se locale foi setado

  /*insere arquivo dicionario na memoria como um
    array de strings */
  dictionary_st dictionary;
  dictionary_set("brazilian", &dictionary);
  /*compara palavras do texto com do dict, e marca palavras
    não encontradas com colchetes, imprime texto na stdout*/
  stream_misspell_check(&dictionary, stdout);
  /*libera memória alocada para dicionário*/
  dictionary_destroy(&dictionary);

  return EXIT_SUCCESS;
}
