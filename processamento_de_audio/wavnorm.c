//GRR20197160 Lucas Müller

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#include "../src/libwav.h"


int main(int argc, char *argv[])
{
  /*realiza a leitura dos argumentos fornecidos pelo usuário,
    -i: desvio de input
    -o: desvio de output*/
  FILE *out_stream = stdout, *inp_stream = stdin;
  for (int i=1; i < argc; ++i){
    if ('-' == argv[i][0]){
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
          fprintf(stderr,"ERRO: Opçao invalida\n\n");
          exit(EXIT_FAILURE);
      }
    }
  }

  wav_st *wav = wav_init(inp_stream);
  
  /*temporário que consegue lidar com amostragens de diferentes bytes por amostra*/
  union {
    int8_t *one_b;
    int16_t *two_b;
    int32_t *four_b;
  } tmp;

  /*seleciona amplitude absoluta máxima atingida entre todas as amostras,
    e normaliza todas a amostras a partir da aplicação de uma fórmula
    relacionando a amplitude máxima encontrada*/
  int64_t max_amplitude = 0;
  switch (wav->fmt.bits_sample){
  case 8: //8 bits
      tmp.one_b = (int8_t*)wav->audio_data.one_b;

      for (int i=0; i < wav->samples_channel; ++i){
        if (max_amplitude < abs(tmp.one_b[i])){
          max_amplitude = abs(tmp.one_b[i]);
        }
      }
      for (int i=0; i < wav->samples_channel; ++i){
        tmp.one_b[i] = round((CHAR_MAX * tmp.one_b[i]) / max_amplitude);
      }
      break;
  case 16: //16 bits
      tmp.two_b = (int16_t*)wav->audio_data.two_b;

      for (int i=0; i < wav->samples_channel; ++i){
        if (max_amplitude < abs(tmp.two_b[i])){
          max_amplitude = abs(tmp.two_b[i]);
        }
      }
      for (int i=0; i < wav->samples_channel; ++i){
        tmp.two_b[i] = round((SHRT_MAX * tmp.two_b[i]) / max_amplitude);
      }
      break;
  case 32: //32 bits
      tmp.four_b = (int32_t*)wav->audio_data.four_b;

      for (int i=0; i < wav->samples_channel; ++i){
        if (max_amplitude < abs(tmp.four_b[i])){
          max_amplitude = abs(tmp.four_b[i]);
        }
      }
      for (int i=0; i < wav->samples_channel; ++i){
        tmp.four_b[i] = round((INT_MAX * tmp.four_b[i]) / max_amplitude);
      }
      break;
  default:
      fprintf(stderr, "\nERRO: bit sample inválido: %d\n\n", wav->fmt.bits_sample);
      exit(EXIT_FAILURE);
  }

  fwrite(wav, 44, 1, out_stream);
  fwrite(wav->audio_data.one_b, wav->data.sub_chunk_2size, 1, out_stream);

  wav_clean(wav);

  fclose(out_stream);
  fclose(inp_stream);

  return EXIT_SUCCESS;
}
