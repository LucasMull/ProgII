//GRR20197160 Lucas Müller

#include <inttypes.h>


typedef struct {
  char chunk_id[4];     //identifica tipo de arquivo
  uint32_t chunk_size;  //tamanho do arquivo em bytes (s/id e size)
  char format[4];       //define formato do restante do conteúdo
} wav_riff_st;

typedef struct {
  char sub_chunk_1id[4];        //cabeçalho do chunk
  uint32_t sub_chunk_1size;     //tamanho deste chunk
  uint16_t audio_format;        //codificação utilizada
  uint16_t num_channels;        //qtd canais de áudio
  uint32_t sample_rate;         //taxa de amostragem (amostras/seg)
  uint32_t byte_rate;           //taxa de bytes por segundo
  uint16_t block_align;         //bytes por amostra (soma dos canais)
  uint16_t bits_sample;         //bits por amostra, por canal
} wav_fmt_st;

typedef struct {
  char sub_chunk_2id[4];        //cabeçalho do chunk;
  uint32_t sub_chunk_2size;     //espaço ocupado pelas amostras(bytes)
} wav_data_st;

typedef union {
  uint8_t  *one_b;      //para 8 bits por amostra
  uint16_t *two_b;      //para 16 bits por amostra
  uint32_t *four_b;     //para 32 bits por amostra
} wav_audio_data_ut;

typedef struct {
  wav_riff_st riff;              //(12 bytes)
  wav_fmt_st fmt;                //(24 bytes)
  wav_data_st data;              //(8 bytes)
  wav_audio_data_ut audio_data;  //amostras de aúdio
  size_t samples_channel;        //qtd amostras por canal
} wav_st;

wav_st* wav_init(FILE* wav_sample);
void wav_clean(wav_st* wav);
