#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <assert.h>

#include "src/dicionario.h"

int main(void)
{
  char *locale=setlocale(LC_CTYPE, "pt_BR.iso88591");
  assert(locale); //verifica se locale foi setado

  /*insere arquivo dicionario na memoria como um
    array de strings */
  dict_t dict;
  set_dict("brazilian", &dict);
  /*compara palavras do texto com do dict, e marca palavras
    não encontradas com colchetes, imprime texto na stdout*/
  mispelling_mark(&dict,stdout);
  /*libera memória alocada para dicionário*/
  destroy_dict(&dict);

  return EXIT_SUCCESS;
}
