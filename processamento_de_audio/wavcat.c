//GRR20197160 Lucas Müller

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "libwav.h"


int main(int argc, char *argv[])
{
  /*realiza a leitura dos argumentos fornecidos pelo usuário,
    -o: desvio de output 
    default: considera que o arg atual se trata de um arquivo .wav a
      ser aberto e manipulado.  */  
  FILE *out_stream = stdout, *inp_stream;
  wav_st *cat = NULL, *wav;
  for (int i=1; i < argc; ++i){
    switch (argv[i][0]){
    case '-':
        if ('o' == argv[i][1]){
          assert(stdout == out_stream); //garante q -o só possa ser chamado uma vez

          out_stream = fopen(argv[++i],"wb");
          assert(NULL != out_stream);
          break;
        }
        fprintf(stderr,"ERRO: opcao invalida ou repetida\n\n");
        exit(EXIT_FAILURE);
    default:
        inp_stream = fopen(argv[i], "rb");
        assert(NULL != inp_stream);

        wav = wav_init(inp_stream);
        
        /*soma as informações referentes à tamanho do header do arquivo
          .wav obtido, com as do arquivo principal (cat), realoca espaço
          de acordo com o novo tamanho de aúdio, e copia o conteúdo do
          arquivo obtido ao final do arquivo original (concatenação)*/
        if (NULL != cat){ 
          assert(cat->fmt.sample_rate == wav->fmt.sample_rate);

          int old_size = cat->data.sub_chunk_2size;

          cat->riff.chunk_size += wav->riff.chunk_size; 
          cat->data.sub_chunk_2size += wav->data.sub_chunk_2size; 
          cat->samples_channel += wav->samples_channel;
          cat->audio_data.one_b = realloc(cat->audio_data.one_b, cat->data.sub_chunk_2size); 
          assert(NULL != cat->audio_data.one_b);

          memcpy(old_size + cat->audio_data.one_b, wav->audio_data.one_b, wav->data.sub_chunk_2size);

          wav_clean(wav);
        } else {
          cat = wav; //primeiro cat simplesmente recebe o wav
        }

        fclose(inp_stream);
        break;
    }
  }

  fwrite(cat, 44, 1, out_stream);
  fwrite(cat->audio_data.one_b, cat->data.sub_chunk_2size, 1, out_stream);

  wav_clean(cat);
  fclose(out_stream);

  return EXIT_SUCCESS;
}
