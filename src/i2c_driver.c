#include "i2c_driver.h"
#include "vu_config.h"

#if I2C_DRIVER == I2C_DRIVER_SYNC
#include "i2c_sync.c"
#elif I2C_DRIVER == I2C_DRIVER_ASYNC
#include "i2c_async.c"
#elif I2C_DRIVER == I2C_DRIVER_PICO
#include "i2c_pico.c"
#endif
