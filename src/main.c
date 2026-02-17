#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

// PICO
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#include "vu_config.h"
#include "utils.h"
#include "i2c_driver.h"
#include "images.h"
#include "oled.h"
#include "vu_meter_i2c.h"

// calibration_data_t CALIBRATION_LEFT EEFIXED = CALIBRATION_INITIALIZER;
// calibration_data_t CALIBRATION_RIGHT EEFIXED = CALIBRATION_INITIALIZER;

vu_meter_i2c_t VU_METER_L;
vu_meter_i2c_t VU_METER_R;

int main(void)
{
  // Initialize the standard I/O
  stdio_init_all();
  sleep_ms(3000);  // Give time for the serial monitor to open
  printf("Init\n");
  i2c_initialize(); 
  // calibration_hw_init();
#if OLED_INCLUDE_RESET
  oled_reset();
#endif

  // LED setup
  gpio_init(25); gpio_set_dir(25, GPIO_OUT);

  vu_meter_i2c_init(
    &VU_METER_L,
    DISPLAY_LEFT_ADDRESS,
    // &CALIBRATION_LEFT,
    DISPLAY_LEFT_FLIPPED,
    DISPLAY_LEFT_BACKGROUND,
    DISPLAY_LEFT_PEAK_INDICATOR, 
    DISPLAY_LEFT_WATERMARK);

//   vu_meter_i2c_init(
//     &VU_METER_R,
//     DISPLAY_RIGHT_ADDRESS,
//     // &CALIBRATION_RIGHT,
//     DISPLAY_RIGHT_FLIPPED,
//     DISPLAY_RIGHT_BACKGROUND,
//     DISPLAY_RIGHT_PEAK_INDICATOR,
//     DISPLAY_RIGHT_WATERMARK);
 
  #if ENABLE_SPLASH_SCREEN
    vu_meter_i2c_splash(&VU_METER_L, DISPLAY_LEFT_SPLASH);
    //vu_meter_i2c_splash(&VU_METER_R, DISPLAY_RIGHT_SPLASH);

    oled_set_display_on(&(VU_METER_L.display.device), true);
    //oled_set_display_on(&(VU_METER_R.display.device), true);

    sleep_ms(SPLASH_SCREEN_TIME_MS);

    oled_set_display_on(&(VU_METER_L.display.device), false);
    //oled_set_display_on(&(VU_METER_R.display.device), false);
  #endif

  bool is_on = false;

  // Seed the random number generator with the current time
  srand(time(NULL));

  sleep_ms(1000);
  printf("Start loop\n");

  for (int i = 0; true; ++i) {
    tight_loop_contents();
    sleep_ms(100);

    // Simulate a random level between 0 and 1023
    static uint16_t max_vu_l = 0;
    uint16_t l_needle = (rand() % 1024);  // rand() gives a range of 0 to RAND_MAX
    max_vu_l = int_max(l_needle, max_vu_l);
    uint16_t l_peak = max_vu_l;
    uint16_t r_needle = l_needle-3;
    uint16_t r_peak = max_vu_l;

    // if (i % 10 == 0) {
    //   adc_reset_peak(true, true);
    //   adc_reset_peak(false, false);
    // }

    vu_meter_i2c_update(&VU_METER_L, l_needle, l_peak);
    //vu_meter_i2c_update(&VU_METER_R, adc_data.r_needle, adc_data.r_peak);

    if (!is_on && i > BLANK_TIME_FRAMES) {
      oled_set_display_on(&(VU_METER_L.display.device), true);
      //oled_set_display_on(&(VU_METER_R.display.device), true);
      is_on = true;
    }

    // LED
    static uint32_t loop_counter = 0;
    static bool blink_led = true;
    if (++loop_counter >= 50) {
        loop_counter = 0;
        // gpio_xor_mask(1u << 25);
        blink_led ? gpio_put(25,0) : gpio_put(25,1);
        blink_led = !blink_led;
        printf("Blink: %d VU L:%d (%d)\n", blink_led, l_needle, max_vu_l);
    }
  }

  while (1);
}
