#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../src/libwav.h"


int main(int argc, char *argv[])
{
  FILE *inp_stream;
  if ((3 == argc) && (0 == strcmp(argv[1],"-i"))){
    inp_stream = fopen(argv[2],"rb");
    assert(NULL != inp_stream);
  } else {
    inp_stream = stdin;
  }

  wav_st *wav = wav_init(inp_stream);
  fclose(inp_stream);

  /* RIFF RELATED */
  fprintf(stderr,"riff tag\t: \"%.*s\"\n", 4, wav->riff.chunk_id);
  fprintf(stderr,"riff size\t: %d\n", wav->riff.chunk_size);
  fprintf(stderr,"wave tag\t: \"%.*s\"\n", 4, wav->riff.format);
  /* FMT RELATED */
  fprintf(stderr,"form tag\t: \"%.*s\"\n", 4, wav->fmt.sub_chunk_1id);
  fprintf(stderr,"fmt_size\t: %d\n", wav->fmt.sub_chunk_1size);
  fprintf(stderr,"audio_format\t: %d\n", wav->fmt.audio_format);
  fprintf(stderr,"num_channels\t: %d\n", wav->fmt.num_channels);
  fprintf(stderr,"sample_rate\t: %d\n", wav->fmt.sample_rate);
  fprintf(stderr,"byte_rate\t: %d\n", wav->fmt.byte_rate);
  fprintf(stderr,"block_align\t: %d\n", wav->fmt.block_align);
  fprintf(stderr,"bits_per_sample\t: %d\n", wav->fmt.bits_sample);
  /* DATA RELATED */
  fprintf(stderr,"data tag\t: \"%.*s\"\n", 4, wav->data.sub_chunk_2id);
  fprintf(stderr,"data size\t: %d\n", wav->data.sub_chunk_2size);
  fprintf(stderr,"samples/channel\t: %ld\n", wav->samples_channel);

  wav_clean(wav);

  return EXIT_SUCCESS;
}
