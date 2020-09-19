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
  wav_st *wav_array[argc]; //no caso de -o incluso vai ter 1 indice a mais (8 bytes apenas)
  wav_st header = {0}; //começa com atributos setados em 0 para identificar primeiro loop
  int num_wav = 0; //total de struct wav apontados pelo wav_array (num_wav < argc)
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

        /* os valores do header do wav atual referente a tamanho
          são somados aos valores anteriores */
        wav_array[num_wav] = wav_init(inp_stream);
        if (0 != header.data.sub_chunk_2size){
          //se sample rate for diferente do header então não é possível concatenar
          assert(header.fmt.sample_rate == wav_array[num_wav]->fmt.sample_rate);

          header.riff.chunk_size += wav_array[num_wav]->riff.chunk_size; 
          header.data.sub_chunk_2size += wav_array[num_wav]->data.sub_chunk_2size; 
          header.samples_channel += wav_array[num_wav]->samples_channel;
        } else { //no primeiro wav lido, header simplesmente recebe uma copia do cabeçalho
          memcpy(&header, wav_array[num_wav], 44);
        }

        ++num_wav;

        fclose(inp_stream);
        break;
    }
  }

  /* escreve cabeçalho com os valores referente ao tamanho total das concatenações calculado, e
    então escreve audio_data de cada wav, em ordem, com o cuidado de liberar da memória após
    cada escrita */
  fwrite(&header, 44, 1, out_stream);
  for (int i=0; i < num_wav; ++i){
    fwrite(wav_array[i]->audio_data.one_b, wav_array[i]->data.sub_chunk_2size, 1, out_stream);
    wav_clean(wav_array[i]);
  }

  fclose(out_stream);

  return EXIT_SUCCESS;
}
