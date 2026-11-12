
# STM32 Libraries

A collection of reusable STM32 peripheral and sensor libraries developed using **STM32 HAL** and **STM32CubeIDE**.

This repository aims to simplify STM32 embedded systems development by providing modular, reusable, and easy-to-integrate drivers for different sensors, displays, and peripherals.

## Features

- Reusable STM32 HAL-based drivers.
- Modular `.c` and `.h` source files.
- Easy integration with STM32CubeIDE projects.
- Support for common communication protocols such as I2C, SPI, and UART.
- Practical examples for learning and testing.
- Continuously expanding library collection.

## Repository Structure

```text
stm32-libraries/
│
├── DHT11/
│   ├── dht11.c
│   └── dht11.h
│
├── MPU6050/
│   ├── mpu6050.c
│   └── mpu6050.h
│
├── BMP280/
│   ├── bmp280.c
│   └── bmp280.h
│
├── OLED_SSD1306/
│   ├── ssd1306.c
│   └── ssd1306.h
│
├── LCD_I2C/
│
├── Examples/
│
├── README.md
├── LICENSE
└── .gitignore
```

## Available Libraries

| Library | Interface | Status |
|---|---|---|
| DHT11 | GPIO | In Development |
| MPU6050 | I2C | In Development |
| BMP280 | I2C / SPI | Planned |
| OLED SSD1306 | I2C / SPI | Planned |
| LCD I2C | I2C | Planned |

*Library availability and implementation status will be updated as development progresses.*

## Requirements

- STM32 microcontroller
- STM32CubeIDE
- STM32 HAL Library
- Basic knowledge of Embedded C
- Appropriate sensor or peripheral hardware

## How to Use

### 1. Clone the Repository

```bash
git clone https://github.com/rajib-mte/stm32-libraries.git
```

### 2. Open Your STM32CubeIDE Project

Create or open an existing STM32CubeIDE project.

### 3. Copy the Required Library

For example, to use the MPU6050 library, copy:

```text
MPU6050/
├── mpu6050.c
└── mpu6050.h
```

Into your STM32 project, for example:

```text
Your_Project/
├── Core/
│   ├── Inc/
│   └── Src/
│
└── Drivers/
    └── MPU6050/
        ├── mpu6050.c
        └── mpu6050.h
```

### 4. Include the Header File

In your `main.c`:

```c
#include "mpu6050.h"
```

### 5. Configure and Initialize

Follow the configuration instructions provided in the README or example files inside each library folder.

## Supported Communication Protocols

- GPIO
- I2C
- SPI
- UART
- PWM

## Development Goals

- Develop reusable STM32 peripheral drivers.
- Maintain clean and modular Embedded C code.
- Support multiple STM32 microcontroller families where practical.
- Provide clear documentation and practical examples.
- Make embedded systems development easier for students and developers.

## Contributions

Contributions, suggestions, bug reports, and improvements are welcome.

If you find a bug or want to contribute:

1. Fork this repository.
2. Create a new branch.
3. Make your changes.
4. Submit a Pull Request.

## Author

**Rajib Hasan**

- GitHub: [@rajib-mte](https://github.com/rajib-mte)
- LinkedIn: [Rajib Hasan](https://www.linkedin.com/in/rajib-mte)

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
