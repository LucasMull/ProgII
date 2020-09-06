//GRR20197160 Lucas Müller

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
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
    if ('-' == argv[i][0]){
      switch (argv[i][1]){
      case 'l':
          set_volume = strtof(argv[++i], NULL);
          if ((set_volume < 0.0) || (set_volume > 10.0)){
            fprintf(stderr,"\nERRO: nível de volume invalido\n\n");
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

  /*realiza a multiplicação em cada canal pelo volume fornecido,
    e ignorando casos em que ocorra overflow/underflow*/
  int64_t tmp;
  switch (wav->fmt.bits_sample){
  case 8: //8 bits
      for (int i=0; i < wav->samples_channel; ++i){
        tmp = set_volume * (int8_t)wav->audio_data.one_b[i]; 
        if (tmp > CHAR_MAX)
          tmp = CHAR_MAX;
        else if (tmp < CHAR_MIN)
          tmp = CHAR_MIN;

        wav->audio_data.one_b[i] = tmp;
      }
      break;
  case 16: //16 bits
      for (int i=0; i < wav->samples_channel; ++i){
        tmp = set_volume * (int16_t)wav->audio_data.two_b[i]; 
        if (tmp > SHRT_MAX)
          tmp = SHRT_MAX;
        else if (tmp < SHRT_MIN)
          tmp = SHRT_MIN;

        wav->audio_data.two_b[i] = tmp;
      }
      break;
  case 32: //32 bits
      for (int i=0; i < wav->samples_channel; ++i){
        tmp = set_volume * (int32_t)wav->audio_data.four_b[i]; 
        if (tmp > INT_MAX)
          tmp = INT_MAX;
        else if (tmp < INT_MIN)
          tmp = INT_MIN;

        wav->audio_data.four_b[i] = tmp;
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
