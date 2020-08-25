#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>

#include "libwav.h"

wav_t*
wav_init(FILE* wav_sample)
{
  wav_t *set_wav = malloc(sizeof(wav_t));
  size_t ret = fread(set_wav, 44, 1, wav_sample);
  assert(ret == 1);

  set_wav->samples_channel = set_wav->data.sub_chunk_2size / set_wav->fmt.block_align;

  set_wav->audio_data = malloc(set_wav->data.sub_chunk_2size);
  fread(set_wav->audio_data, set_wav->data.sub_chunk_2size, 1, wav_sample);

  return set_wav;
}

void
wav_clean(wav_t* wav)
{
  if (wav->audio_data != NULL)
    free(wav->audio_data);
  free(wav);
}
