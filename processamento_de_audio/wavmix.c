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
  wav_st *mix = NULL, *wav;
  for (int i=1; i < argc; ++i){
    switch (argv[i][0]){
    case '-':
        if ('o' != argv[i][1]){
          fprintf(stderr,"ERRO: opcao invalida ou repetida\n\n");
          exit(EXIT_FAILURE);
        }

        assert(stdout == out_stream); //garante q -o só possa ser chamado uma vez
        out_stream = fopen(argv[++i],"wb");
        assert(NULL != out_stream);
        break;
    default:
        inp_stream = fopen(argv[i], "rb");
        assert(NULL != inp_stream);

        wav = wav_init(inp_stream);

        /*soma as samples do arquivo .wav obtido pelo argumento, com
          as do arquivo principal (mix), não há um limite de quantas
          somas podem ser realizadas no mix*/
        if (NULL != mix){ 
          assert(mix->fmt.sample_rate == wav->fmt.sample_rate);
          /* se o arquivo obtido for maior do que o principal,
            realizar um simples swap entre os dois, para evitar
            stack overflow na soma das samples */
          if (mix->riff.chunk_size < wav->riff.chunk_size){
            wav_st *tmp = mix;
            mix = wav;
            wav = tmp;
          }
          /* baseada na equação de mixagem de aúdio de Viktor T. Toth
            http://www.vttoth.com/CMS/index.php/technical-notes/68 */
          int_fast64_t tmp_a, tmp_b, tmp_res;
          for (int j=0; j < wav->data.sub_chunk_2size; ++j){
            tmp_a = INT8_MAX+1 + (int8_t)mix->audio_data.one_b[j];
            tmp_b = INT8_MAX+1 + (int8_t)wav->audio_data.one_b[j];

            if ((tmp_a < INT8_MAX+1) || (tmp_b < INT8_MAX+1)){
              tmp_res = tmp_a * tmp_b / (INT8_MAX+1);
            } else {
              tmp_res = (2 * (tmp_a + tmp_b) - (tmp_a * tmp_b) / (INT8_MAX+1)) - UINT8_MAX+1;
            }

            if (UINT8_MAX+1 == tmp_res)
              tmp_res = UINT8_MAX;
            tmp_res -= INT8_MAX+1;

            mix->audio_data.one_b[j] = tmp_res; 
          }

          wav_clean(wav);
        } else {
          mix = wav; //primeiro mix simplesmente recebe o wav
        }

        fclose(inp_stream);
        break;
    }
  }

  fwrite(mix, 44, 1, out_stream);
  fwrite(mix->audio_data.one_b, mix->data.sub_chunk_2size, 1, out_stream);

  wav_clean(mix);
  fclose(out_stream);

  return EXIT_SUCCESS;
}
