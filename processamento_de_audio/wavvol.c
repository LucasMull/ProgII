//GRR20197160 Lucas Müller

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "libwav.h"


int main(int argc, char *argv[])
{
  /*realiza a leitura dos argumentos fornecidos pelo usuário,
    -l: nível relativo do volume
    -i: desvio de input  
    -o: desvio de output */  
  FILE *out_stream = stdout, *inp_stream = stdin;
  float set_volume = 1.0;
  for (int i=1; i < argc; ++i){
    if ('-' != argv[i][0]){
      fprintf(stderr,"\nERRO: opcao invalida\n\n");
      exit(EXIT_FAILURE);
    }

    switch (argv[i][1]){
    case 'l':
        set_volume = strtof(argv[++i], NULL);
        if ((set_volume < 0.0) || (set_volume > 10.0)){
          fprintf(stderr,"\nERRO: nivel de volume invalido\n\n");
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
        fprintf(stderr,"\nERRO: opcao invalida\n\n");
        exit(EXIT_FAILURE);
    }
  }

  wav_st *wav = wav_init(inp_stream);

  /*realiza a multiplicação em cada canal pelo volume fornecido,
    e trata devidamente casos de overflow/underflow*/
  int_fast64_t tmp;
  switch (wav->fmt.bits_sample){
  case 8: //8 bits
      for (int i=0; i < wav->samples_channel; ++i){
        tmp = set_volume * (int8_t)wav->audio_data.one_b[i]; 
        if (tmp > INT8_MAX)
          tmp = INT8_MAX;
        else if (tmp < INT8_MIN)
          tmp = INT8_MIN;

        wav->audio_data.one_b[i] = tmp;
      }
      break;
  case 16: //16 bits
      for (int i=0; i < wav->samples_channel; ++i){
        tmp = set_volume * (int16_t)wav->audio_data.two_b[i]; 
        if (tmp > INT16_MAX)
          tmp = INT16_MAX;
        else if (tmp < INT16_MIN)
          tmp = INT16_MIN;

        wav->audio_data.two_b[i] = tmp;
      }
      break;
  case 32: //32 bits
      for (int i=0; i < wav->samples_channel; ++i){
        tmp = set_volume * (int32_t)wav->audio_data.four_b[i]; 
        if (tmp > INT32_MAX)
          tmp = INT32_MAX;
        else if (tmp < INT32_MIN)
          tmp = INT32_MIN;

        wav->audio_data.four_b[i] = tmp;
      }
      break;
  default:
      fprintf(stderr, "\nERRO: bit sample invalido: %d\n\n", wav->fmt.bits_sample);
      exit(EXIT_FAILURE);
  }

  fwrite(wav, 44, 1, out_stream);
  fwrite(wav->audio_data.one_b, wav->data.sub_chunk_2size, 1, out_stream);

  wav_clean(wav);

  fclose(out_stream);
  fclose(inp_stream);

  return EXIT_SUCCESS;
}
