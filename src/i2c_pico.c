#include <stdio.h>
#include "vu_config.h"
#include "i2c_driver.h"

// PICO
#include "pico/stdlib.h"
#include "pico/unique_id.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

static uint8_t I2C_STATUS = TW_OK;

static void
i2c_start(void)
{
  // i2c_hw_send_start_condition();
  // i2c_hw_wait_for_int();
  // I2C_STATUS = i2c_hw_get_status();
  I2C_STATUS = TW_OK;
}


static void
i2c_finish(void)
{
  // i2c_hw_send_stop_condition();
  // i2c_hw_go_idle();
}


static void
i2c_send_byte(uint8_t val)
{
  // i2c_hw_send_byte(data);
  // i2c_hw_wait_for_int();
  // I2C_STATUS = i2c_hw_get_status();
  I2C_STATUS = TW_OK;
}


void i2c_initialize(void)
{
  // i2c_hw_init();
  i2c_init(i2c0, 400000);

  gpio_set_function(PICO_VU_1_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_VU_2_I2C_SCK_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_VU_1_I2C_SDA_PIN);
  gpio_pull_up(PICO_VU_2_I2C_SCK_PIN);
  sleep_ms(500);
}

uint8_t
i2c_wait(void)
{
  return I2C_STATUS;
}

/**
 * @param data First byte is address
 */
void i2c_transmit(uint16_t length, uint8_t *data)
{
  switch(i2c_write_blocking(i2c0, data[0], &data[1], length, false)) {
  case PICO_ERROR_GENERIC:
      printf("addr %x not acknowledged!\n", data[0]);
      break;
  case PICO_ERROR_TIMEOUT:
      printf("timeout!\n");
      break;
  default:
      //printf("[%s] wrote successfully %lu bytes!\n", name, len);
      break;
  }
  // i2c_start();
  // check_status();

  // for (uint16_t i = 0; i < length; ++i) {
  //   i2c_send_byte(data[i]);
  //   check_status();
  // }

  // finish: i2c_finish();
}


/**
 * @param address I2C address to send this data to
 * @param data Does not include address
 */
uint8_t i2c_transmit_progmem(uint8_t address, uint16_t length, const uint8_t *data)
{
  switch(i2c_write_blocking(i2c0, address, data, length, false)) {
  case PICO_ERROR_GENERIC:
      printf("addr not acknowledged!\n");
      break;
  case PICO_ERROR_TIMEOUT:
      printf("timeout!\n");
      break;
  default:
      //printf("[%s] wrote successfully %lu bytes!\n", name, len);
      break;
  }

  // i2c_start();
  // check_status();

  // i2c_send_byte(address);
  // check_status();

  // for (uint16_t i = 0; i < length; ++i) {
  //   uint8_t byte = pgm_read_byte(data + i);
  //   i2c_send_byte(byte);
  //   check_status();
  // }

  // finish: i2c_finish();
  return I2C_STATUS;
}
