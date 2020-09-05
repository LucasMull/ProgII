//GRR20197160 Lucas Müller

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>

#include "libwav.h"


wav_st*
wav_init(FILE* wav_sample)
{
  wav_st *set_wav = malloc(sizeof *set_wav);
  assert(NULL != set_wav);

  size_t ret = fread(set_wav, 44, 1, wav_sample);
  assert(1 == ret);

  set_wav->audio_data.one_b = malloc(set_wav->data.sub_chunk_2size);
  assert(NULL != set_wav->audio_data.one_b);

  fread(set_wav->audio_data.one_b, set_wav->data.sub_chunk_2size, 1, wav_sample);

  set_wav->samples_channel = set_wav->data.sub_chunk_2size / set_wav->fmt.block_align;


  return set_wav;
}

void
wav_clean(wav_st* wav)
{
  if (NULL != wav->audio_data.one_b)
    free(wav->audio_data.one_b);
  free(wav);
}
