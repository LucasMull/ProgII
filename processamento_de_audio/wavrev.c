//GRR20197160 Lucas Müller

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "libwav.h"


int main(int argc, char *argv[])
{
  /*realiza a leitura dos argumentos fornecidos pelo usuário,
    -i: desvio de input  
    -o: desvio de output */  
  FILE *out_stream = stdout, *inp_stream = stdin;
  for (int i=1; i < argc; ++i){
    if ('-' != argv[i][0]){
      fprintf(stderr,"ERRO: opçao invalida\n\n");
      exit(EXIT_FAILURE);
    }

    switch (argv[i][1]){
      case 'i':
        inp_stream = fopen(argv[++i],"rb");
        assert(NULL != inp_stream);
        break;
      case 'o':
        out_stream = fopen(argv[++i],"wb");
        assert(NULL != out_stream);
        break;
      default:
        fprintf(stderr,"ERRO: opçao invalida\n\n");
        exit(EXIT_FAILURE);
    }
  }

  wav_st *wav = wav_init(inp_stream);

  uint8_t *reverse_audio_data = malloc(wav->data.sub_chunk_2size);
  assert(NULL != reverse_audio_data);
  /*para este laço, a leitura é feita byte a byte, portanto para
    conciliar .wav de diferentes qtd de bits por canal, é necessário
    reservar uma variável bytes_sample para se movimentar 
    apropriadamente pelos índices.
    
    sample_index indica quando começa a leitura de uma nova amostra,
    e index cálcula um índice para fazer o armazenamento inverso*/
  int index;
  int sample_index = 0;
  int bytes_sample = wav->fmt.bits_sample / 8;
  for (int i=0; i < wav->data.sub_chunk_2size; ++i){
    if ((0 != i) && (0 == i % bytes_sample)){
      sample_index += 2 * bytes_sample;
    }
    index = wav->data.sub_chunk_2size - bytes_sample - sample_index + i;
    reverse_audio_data[i] = wav->audio_data.one_b[index];
  }

  fwrite(wav, 44, 1, out_stream);
  fwrite(reverse_audio_data, wav->data.sub_chunk_2size, 1, out_stream);

  free(reverse_audio_data);
  wav_clean(wav);

  fclose(out_stream);
  fclose(inp_stream);

  return EXIT_SUCCESS;
}
