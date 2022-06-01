#include <Arduino.h>
/*
 * Buzzer part start rewrite from:
 * https://github.com/moononournation/esp_8_bit.git
 */
#include <driver/i2s.h>
#include <esp32-hal-timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <nes/nes.h>
#include <soc/ledc_struct.h>

#define DEFAULT_FRAGSIZE 64
static void (*audio_callback)(void *buffer, int length) = NULL;
QueueHandle_t queue;
static int16_t *audio_frame;

int osd_init_sound() {
  audio_frame = NOFRENDO_MALLOC(4 * DEFAULT_FRAGSIZE);
//  i2s_config_t cfg = {
//      .mode = I2S_MODE_MASTER | I2S_MODE_TX,
//      .sample_rate = HW_AUDIO_SAMPLERATE,
//      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
//      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
//      .communication_format = I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB,
//      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
//      .dma_buf_count = 7,
//      .dma_buf_len = 256,
//      .use_apll = false,
//  };
//  i2s_driver_install(I2S_NUM_0, &cfg, 2, &queue);
//  i2s_pin_config_t pins = {
//      .bck_io_num = HW_AUDIO_EXTDAC_BCLK,
//      .ws_io_num = HW_AUDIO_EXTDAC_WCLK,
//      .data_out_num = HW_AUDIO_EXTDAC_DOUT,
//      .data_in_num = I2S_PIN_NO_CHANGE,
//  };
//  i2s_set_pin(I2S_NUM_0, &pins);
//  i2s_zero_dma_buffer(I2S_NUM_0);
  audio_callback = NULL;
  return 0;
}

void osd_stopsound() { audio_callback = NULL; }

void do_audio_frame() {
  int left = HW_AUDIO_SAMPLERATE / NES_REFRESH_RATE;
  while (left) {
    int n = DEFAULT_FRAGSIZE;
    if (n > left) n = left;
    audio_callback(audio_frame, n);  // get more data

    // 16 bit mono -> 32-bit (16 bit r+l)
    int16_t *mono_ptr = audio_frame + n;
    int16_t *stereo_ptr = audio_frame + n + n;
    int i = n;
    while (i--) {
      int16_t a = (*(--mono_ptr) >> 2);
      *(--stereo_ptr) = a;
      *(--stereo_ptr) = a;
    }

    size_t i2s_bytes_write;
    i2s_write(I2S_NUM_0, (const char *)audio_frame, 4 * n, &i2s_bytes_write,
              portMAX_DELAY);
    left -= i2s_bytes_write / 4;
  }
}

void osd_setsound(void (*playfunc)(void *buffer, int length)) {
  // Indicates we should call playfunc() to get more data.
  audio_callback = playfunc;
}

void osd_getsoundinfo(sndinfo_t *info) {
  info->sample_rate = HW_AUDIO_SAMPLERATE;
  info->bps = 16;
}