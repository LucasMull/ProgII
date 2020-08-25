#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "libwav.h"

int main(int argc, char *argv[])
{
  wav_t *wav = wav_init(stdin);
  fwrite(&wav->riff,sizeof wav->riff,1,stdout);
  fputc('\n',stdout);
  fwrite(&wav->fmt,sizeof wav->fmt,1,stdout);
  fputc('\n',stdout);

  wav_clean(wav);

  return EXIT_SUCCESS;
}
