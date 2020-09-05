//GRR20197160 Lucas Müller

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "../src/libwav.h"


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
        if ('o' == argv[i][1]){
          assert(stdout == out_stream); //garante q -o só possa ser chamado uma vez

          out_stream = fopen(argv[++i],"wb");
          assert(NULL != out_stream);
          break;
        }
        fprintf(stderr,"ERRO: Opçao invalida ou repetida\n\n");
        exit(EXIT_FAILURE);
    default:
        inp_stream = fopen(argv[i], "rb");
        assert(NULL != inp_stream);

        wav = wav_init(inp_stream);

        /*soma as samples do arquivo .wav obtido pelo argumento, com
          as do arquivo principal (mix), não há um limite de quantas
          somas podem ser realizadas no mix*/
        if (NULL != mix){ 
          /*se o arquivo obtido for maior do que o principal, realocar
            espaço para que o original tenha um valor exato ao do obtido
            e atualizar informações do header*/
          assert(mix->fmt.sample_rate == wav->fmt.sample_rate);
          if (mix->riff.chunk_size < wav->riff.chunk_size){
            int old_size = mix->data.sub_chunk_2size;

            mix->riff.chunk_size = wav->riff.chunk_size; 
            mix->data.sub_chunk_2size = wav->data.sub_chunk_2size; 
            mix->samples_channel = wav->samples_channel;
            mix->audio_data.one_b = realloc(mix->audio_data.one_b, mix->data.sub_chunk_2size); 
            assert(NULL != mix->audio_data.one_b);

            //seta 0 em todo espaço novo alocado pra não somar com lixo de memória
            memset(&mix->audio_data.one_b[old_size], 0, mix->data.sub_chunk_2size - old_size);
          }
         
          int32_t tmp_a, tmp_b, tmp_res;
          /* baseada na equação de mixagem de aúdio de Viktor T. Toth
            http://www.vttoth.com/CMS/index.php/technical-notes/68 */
          for (int j=0; j < wav->data.sub_chunk_2size; ++j){
            tmp_a = CHAR_MAX+1 + (int32_t)mix->audio_data.one_b[j];
            tmp_b = CHAR_MAX+1 + (int32_t)wav->audio_data.one_b[j];

            if ((tmp_a < CHAR_MAX+1) || (tmp_b < CHAR_MAX+1)){
              tmp_res = tmp_a * tmp_b / (CHAR_MAX+1);
            } else {
              tmp_res = 2 * (tmp_a + tmp_b) - (tmp_a * tmp_b) / (CHAR_MAX+1) - UCHAR_MAX+1;
            }

            if (UCHAR_MAX+1 == tmp_res)
              tmp_res = UCHAR_MAX;
            tmp_res -= CHAR_MAX+1;

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
