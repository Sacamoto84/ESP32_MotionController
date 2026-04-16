#include <EncButton.h>

#include "driver/pulse_cnt.h"

#define BTN 4
#define ENC_A 5
#define ENC_B 6

EncButton eb(ENC_A, ENC_B, BTN);

static pcnt_unit_handle_t pcnt_unit = NULL;
static pcnt_channel_handle_t pcnt_chan = NULL;

void encoderInit()
{
    pinMode(BTN, INPUT);
    pinMode(ENC_A, INPUT);
    pinMode(ENC_B, INPUT);

    // // Создаем unit
    // pcnt_unit_config_t unit_config = {.low_limit = -1000, .high_limit = 1000, .intr_priority = 0, .flags = {.accum_count = 1}};

    // ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));

    // // Фильтр от дребезга
    // pcnt_glitch_filter_config_t filter_config = {
    //     .max_glitch_ns = 10000 // начни с 1000 нс, потом подберем
    // };
    // ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));

    // // Один канал: A = edge, B = level(control)
    // pcnt_chan_config_t chan_config = {
    //     .edge_gpio_num = ENC_A,
    //     .level_gpio_num = ENC_B,
    //     .flags = {
    //         .invert_edge_input = 0,
    //         .invert_level_input = 0,
    //         .virt_edge_io_level = 0,
    //         .virt_level_io_level = 0,
    //         .io_loop_back = 0}};

    // ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_config, &pcnt_chan));

    // // Что делать на фронтах A
    // // rising: +1, falling: -1
    // ESP_ERROR_CHECK(
    //     pcnt_channel_set_edge_action(
    //         pcnt_chan,
    //         PCNT_CHANNEL_EDGE_ACTION_INCREASE,
    //         PCNT_CHANNEL_EDGE_ACTION_DECREASE));

    // // Что делать в зависимости от уровня B
    // // low: как есть
    // // high: инвертировать направление
    // ESP_ERROR_CHECK(
    //     pcnt_channel_set_level_action(
    //         pcnt_chan,
    //         PCNT_CHANNEL_LEVEL_ACTION_KEEP,
    //         PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

    // ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    // ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
    // ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));
}



/**
 * 
 * int count = 0;

void loop() {
  int val = 0;
  pcnt_unit_get_count(pcnt_unit, &val);

  if (val > 50) {
    count += val;
    pcnt_unit_clear_count(pcnt_unit);
  }
  else if (val < -50) {
    count += val;
    pcnt_unit_clear_count(pcnt_unit);
  }

  Serial.println(count);
}
 * 
 * 
 */