
#ifndef DHT11_H
#define DHT11_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

/* DHT11 Status */
typedef enum
{
    DHT11_OK = 0,
    DHT11_ERROR,
    DHT11_TIMEOUT,
    DHT11_CHECKSUM_ERROR

} DHT11_Status;

/* DHT11 Data Structure */
typedef struct
{
    uint8_t temperature;
    uint8_t humidity;

} DHT11_Data;

/* Function Prototypes */

void DHT11_Init(void);

DHT11_Status DHT11_Read(
    GPIO_TypeDef *GPIOx,
    uint16_t GPIO_Pin,
    DHT11_Data *data
);

#endif
