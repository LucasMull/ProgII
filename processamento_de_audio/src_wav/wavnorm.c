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

  for (int i=1; i < argc; ++i){
    if (argv[i][0] == '-'){
      switch (argv[i][1]){
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

  wav_t *wav = wav_init(inp_stream);

  int16_t *audio_data = (int16_t*)wav->twob_audio_data;

  int16_t max_amplitude = 0;
  for (int i=0; i < wav->samples_channel; ++i){
    if (max_amplitude < abs(audio_data[i])){
      max_amplitude = abs(audio_data[i]);
    }
  }

  for (int i=0; i < wav->samples_channel; ++i){
    audio_data[i] = round((SHRT_MAX * audio_data[i]) / max_amplitude);
  }

  fwrite(wav, 44, 1, out_stream);
  fwrite(audio_data, wav->data.sub_chunk_2size, 1, out_stream);

  wav_clean(wav);

  fclose(out_stream);
  fclose(inp_stream);

  return EXIT_SUCCESS;
}
