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

  switch (set_wav->fmt.bits_sample){
    case 32: //32 bits
      set_wav->fourb_audio_data = malloc(set_wav->data.sub_chunk_2size);
      fread(set_wav->fourb_audio_data, set_wav->data.sub_chunk_2size, 1, wav_sample);
      break;
    case 16: //16 bits
      set_wav->twob_audio_data = malloc(set_wav->data.sub_chunk_2size);
      fread(set_wav->twob_audio_data, set_wav->data.sub_chunk_2size, 1, wav_sample);
      break;
  }

  set_wav->samples_channel = set_wav->data.sub_chunk_2size / set_wav->fmt.block_align;


  return set_wav;
}

void
wav_clean(wav_t* wav)
{
  if (wav->twob_audio_data != NULL)
    free(wav->twob_audio_data);
  free(wav);
}
