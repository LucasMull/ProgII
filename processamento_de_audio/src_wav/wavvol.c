#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#include "../src/libwav.h"

int main(int argc, char *argv[])
{
  FILE *out_stream = stdout, *inp_stream = stdin;
  float set_volume = 1.0;

  for (int i=1; i < argc; ++i){
    if ('-' == argv[i][0]){
      switch (argv[i][1]){
        case 'l':
          set_volume = strtof(argv[++i], NULL);
          if ((set_volume < 0.0) || (set_volume > 10.0)){
            fprintf(stderr,"ERRO: valor de volume invalido\n\n");
            exit(EXIT_FAILURE);
          }
          break;
        case 'i':
          inp_stream = fopen(argv[++i],"rb");
          assert(inp_stream);
          break;
        case 'o':
          out_stream = fopen(argv[++i],"wb");
          assert(out_stream);
          break;
        default:
          fprintf(stderr,"ERRO: Opçao invalida\n\n");
          exit(EXIT_FAILURE);
      }
    }
  }

  wav_st *wav = wav_init(inp_stream);

  int64_t tmp;
  switch (wav->fmt.bits_sample){
    case 32: //32 bits
      for (int i=0; i < wav->samples_channel; ++i){
        tmp = set_volume * (int32_t)wav->fourb_audio_data[i]; 
        if ((tmp < INT_MAX) && (tmp > INT_MIN)){
          wav->twob_audio_data[i] = tmp;
        }
      }
      fwrite(wav, 44, 1, out_stream);
      fwrite(wav->fourb_audio_data, wav->data.sub_chunk_2size, 1, out_stream);
      break;
    case 16: //16 bits
      for (int i=0; i < wav->samples_channel; ++i){
        tmp = set_volume * (int16_t)wav->twob_audio_data[i]; 
        if ((tmp < SHRT_MAX) && (tmp > SHRT_MIN)){
          wav->twob_audio_data[i] = tmp;
        }
      }
      fwrite(wav, 44, 1, out_stream);
      fwrite(wav->twob_audio_data, wav->data.sub_chunk_2size, 1, out_stream);
      break;
  }

  wav_clean(wav);

  fclose(out_stream);
  fclose(inp_stream);

  return EXIT_SUCCESS;
}
