
#include "dht11.h"

/* Private function declarations */

static void DHT11_DelayUs(uint32_t us);
static void DHT11_SetPinOutput(GPIO_TypeDef *GPIOx,
                               uint16_t GPIO_Pin);
static void DHT11_SetPinInput(GPIO_TypeDef *GPIOx,
                              uint16_t GPIO_Pin);

static DHT11_Status DHT11_WaitForPin(
    GPIO_TypeDef *GPIOx,
    uint16_t GPIO_Pin,
    GPIO_PinState state,
    uint32_t timeout
);

/* Initialize DWT cycle counter */

void DHT11_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    DWT->CYCCNT = 0;

    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

/* Microsecond delay */

static void DHT11_DelayUs(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;

    uint32_t ticks = us * (SystemCoreClock / 1000000U);

    while ((DWT->CYCCNT - start) < ticks);
}

/* Configure GPIO as output open-drain */

static void DHT11_SetPinOutput(GPIO_TypeDef *GPIOx,
                               uint16_t GPIO_Pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/* Configure GPIO as input */

static void DHT11_SetPinInput(GPIO_TypeDef *GPIOx,
                              uint16_t GPIO_Pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/* Wait for a particular GPIO state */

static DHT11_Status DHT11_WaitForPin(
    GPIO_TypeDef *GPIOx,
    uint16_t GPIO_Pin,
    GPIO_PinState state,
    uint32_t timeout)
{
    uint32_t start = DWT->CYCCNT;

    uint32_t ticks = timeout *
                     (SystemCoreClock / 1000000U);

    while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) != state)
    {
        if ((DWT->CYCCNT - start) > ticks)
        {
            return DHT11_TIMEOUT;
        }
    }

    return DHT11_OK;
}

/* Read DHT11 Sensor */

DHT11_Status DHT11_Read(
    GPIO_TypeDef *GPIOx,
    uint16_t GPIO_Pin,
    DHT11_Data *data)
{
    uint8_t rawData[5] = {0};

    if (data == NULL)
        return DHT11_ERROR;

    /* Start signal: MCU pulls DATA low */

    DHT11_SetPinOutput(GPIOx, GPIO_Pin);

    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);

    HAL_Delay(20);

    /* Release DATA line */

    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);

    DHT11_DelayUs(30);

    DHT11_SetPinInput(GPIOx, GPIO_Pin);

    /* Sensor response: LOW -> HIGH -> LOW */

    if (DHT11_WaitForPin(GPIOx, GPIO_Pin,
                         GPIO_PIN_RESET, 100) != DHT11_OK)
        return DHT11_TIMEOUT;

    if (DHT11_WaitForPin(GPIOx, GPIO_Pin,
                         GPIO_PIN_SET, 100) != DHT11_OK)
        return DHT11_TIMEOUT;

    if (DHT11_WaitForPin(GPIOx, GPIO_Pin,
                         GPIO_PIN_RESET, 100) != DHT11_OK)
        return DHT11_TIMEOUT;

    /* Read 40 bits */

    for (uint8_t i = 0; i < 40; i++)
    {
        /* Wait for HIGH pulse */

        if (DHT11_WaitForPin(GPIOx, GPIO_Pin,
                             GPIO_PIN_SET, 100) != DHT11_OK)
            return DHT11_TIMEOUT;

        /* Measure HIGH pulse duration */

        uint32_t start = DWT->CYCCNT;

        while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)
               == GPIO_PIN_SET)
        {
            if ((DWT->CYCCNT - start) >
                (100U * (SystemCoreClock / 1000000U)))
            {
                return DHT11_TIMEOUT;
            }
        }

        uint32_t pulseWidth = DWT->CYCCNT - start;

        /* HIGH pulse around 26-28us = 0
           HIGH pulse around 70us = 1 */

        rawData[i / 8] <<= 1;

        if (pulseWidth >
            (40U * (SystemCoreClock / 1000000U)))
        {
            rawData[i / 8] |= 1;
        }
    }

    /* Check checksum */

    uint8_t checksum =
        rawData[0] + rawData[1] +
        rawData[2] + rawData[3];

    if (checksum != rawData[4])
    {
        return DHT11_CHECKSUM_ERROR;
    }

    /* Store sensor data */

    data->humidity = rawData[0];
    data->temperature = rawData[2];

    return DHT11_OK;
}
