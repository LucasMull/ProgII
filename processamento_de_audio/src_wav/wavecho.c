#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "../src/libwav.h"


int main(int argc, char *argv[])
{
  FILE *out_stream = stdout, *inp_stream = stdin;
  float echo_rate = 0.5;
  long delay_ms = 1000;
  for (int i=1; i < argc; ++i){
    if ('-' == argv[i][0]){
      switch (argv[i][1]){
      case 't':
          delay_ms = strtol(argv[++i], NULL, 10);
          if ((0 > delay_ms) || (UINT_MAX < delay_ms)){
            fprintf(stderr,"\nERRO: delay menor que 0 ou overflow\n\n");
          }
          break;
      case 'l':
          echo_rate = strtof(argv[++i], NULL);
          if ((echo_rate < 0.0) || (echo_rate > 1.0)){
            fprintf(stderr,"\nERRO: level de eco invalido\n\n");
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
  
  int8_t *clone = malloc(wav->data.sub_chunk_2size);
  memcpy(clone, wav->audio_data.one_b, wav->data.sub_chunk_2size);
  
  int16_t tmp;
  for (int i = delay_ms + 1; i < wav->data.sub_chunk_2size; ++i){
    tmp = (uint8_t)(clone[i] + echo_rate * clone[i-delay_ms]);
    if ((SHRT_MIN < tmp) && (SHRT_MAX > tmp)){
      wav->audio_data.one_b[i] = tmp;
    }
  }

  fwrite(wav, 44, 1, out_stream);
  fwrite(wav->audio_data.one_b, wav->data.sub_chunk_2size, 1, out_stream);

  free(clone);
  wav_clean(wav);

  fclose(out_stream);
  fclose(inp_stream);

  return EXIT_SUCCESS;
}
