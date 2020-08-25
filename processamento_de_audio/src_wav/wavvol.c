#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../src/libwav.h"

int main(int argc, char *argv[])
{
  FILE *out_stream=NULL, *inp_stream=NULL;
  long set_volume = 1;

  for (int i=1; i < argc; ++i){
    if (argv[i][0] == '-'){
      switch (argv[i][1]){
        case 'l':
          set_volume = strtol(argv[i+1], NULL, 10);
          break;
        case 'i':
          inp_stream = fopen(argv[i+1],"rb");
          assert(inp_stream);
          break;
        case 'o':
          out_stream = fopen(argv[i+1],"wb");
          assert(out_stream);
          break;
        default:
          fprintf(stderr,"ERRO: Opçao invalida\n\n");
          exit(EXIT_FAILURE);
      }
      ++i; //pula o argumento seguinte (já foi utilizado)
    }
  }

  if (!out_stream)
    out_stream = stdout;
  if (!inp_stream)
    inp_stream = stdin;

  wav_t *wav = wav_init(inp_stream);
  //fix: aceitar floats na multiplicação
  for (int i=0; i < wav->samples_channel; ++i){
    wav->audio_data[i] *= set_volume; 
  }

  fwrite(wav, 44, 1, out_stream);
  fwrite(wav->audio_data, wav->fmt.block_align, wav->samples_channel, out_stream);

  wav_clean(wav);

  fclose(out_stream);
  fclose(inp_stream);

  return EXIT_SUCCESS;
}
