#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

  int bytes_per_sample = wav->fmt.bits_sample >> 3;
  int sample_index = 0;

  uint8_t *reverse_audio_data = malloc(wav->data.sub_chunk_2size);
  uint8_t *forward_audio_data = (uint8_t*)wav->twob_audio_data;

  int index;
  for (int i=0; i < wav->data.sub_chunk_2size; ++i){
    if ((i != 0) && (i % bytes_per_sample == 0)){
      sample_index += 2*bytes_per_sample;
    }
    index = wav->data.sub_chunk_2size - bytes_per_sample - sample_index + i;
    reverse_audio_data[i] = forward_audio_data[index];
  }

  fwrite(wav, 44, 1, out_stream);
  fwrite(reverse_audio_data, wav->data.sub_chunk_2size, 1, out_stream);

  free(reverse_audio_data);
  wav_clean(wav);

  fclose(out_stream);
  fclose(inp_stream);

  return EXIT_SUCCESS;
}