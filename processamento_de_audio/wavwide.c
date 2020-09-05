//GRR20197160 Lucas Müller

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "libwav.h"


int main(int argc, char *argv[])
{
  /*realiza a leitura dos argumentos fornecidos pelo usuário,
    -l: fator de amplificação
    -i: desvio de input
    -o: desvio de output*/
  FILE *out_stream = stdout, *inp_stream = stdin;
  float amplify_ratio = 1.0;
  for (int i=1; i < argc; ++i){
    if ('-' == argv[i][0]){
      switch (argv[i][1]){
      case 'l':
          amplify_ratio = strtof(argv[++i], NULL);
          if ((amplify_ratio < 0.0) || (amplify_ratio > 10.0)){
            fprintf(stderr,"\nERRO: fator de amplificacao invalido\n\n");
            exit(EXIT_FAILURE);
          }
          break;
      case 'i':
          inp_stream = fopen(argv[++i],"rb");
          assert(NULL != inp_stream);
          break;
      case 'o':
          out_stream = fopen(argv[++i],"wb");
          assert(NULL != out_stream);
          break;
      default:
          fprintf(stderr,"\nERRO: Opçao invalida\n\n");
          exit(EXIT_FAILURE);
      }
    }
  }

  wav_st *wav = wav_init(inp_stream);
  assert(2 == wav->fmt.num_channels); //só funciona com estéreo
  assert(16 == wav->fmt.bits_sample);
  
  /*pelo fator de amplificação fornecido, e pela diferença entre os canais
  esquerdo e direito, é gerado um produto que ao ser subtraido/somado com
  os canais esquerdo e direito respecticamente resulta num efeito de
  "distanciamento" entre as saídas estéreo*/
  int64_t diff;
  int64_t l_tmp, r_tmp;
  for (int i=0; i < wav->samples_channel; i += 2){
    l_tmp = (int64_t)wav->audio_data.two_b[i];
    r_tmp = (int64_t)wav->audio_data.two_b[i+1];

    diff = r_tmp - l_tmp;

    l_tmp -= amplify_ratio * diff;
    wav->audio_data.two_b[i] = l_tmp;

    r_tmp += amplify_ratio * diff;
    wav->audio_data.two_b[i+1] = r_tmp;
  }

  fwrite(wav, 44, 1, out_stream);
  fwrite(wav->audio_data.one_b, wav->data.sub_chunk_2size, 1, out_stream);

  wav_clean(wav);

  fclose(out_stream);
  fclose(inp_stream);

  return EXIT_SUCCESS;
}
