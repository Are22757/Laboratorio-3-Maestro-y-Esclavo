#ifndef SPI_M_H_
#define SPI_M_H_

#include <avr/io.h>
#include <stdint.h>

// Definición de los tipos y configuraciones para SPI

// Tipos de configuración del SPI
typedef enum {
	SPI_MASTER_OSC_DIV2   = 0b01010000, // Maestro, reloj dividido por 2
	SPI_MASTER_OSC_DIV4   = 0b01010001, // Maestro, reloj dividido por 4
	SPI_MASTER_OSC_DIV8   = 0b01010010, // Maestro, reloj dividido por 8
	SPI_MASTER_OSC_DIV16  = 0b01010011, // Maestro, reloj dividido por 16
	SPI_MASTER_OSC_DIV32  = 0b01010100, // Maestro, reloj dividido por 32
	SPI_MASTER_OSC_DIV64  = 0b01010101, // Maestro, reloj dividido por 64
	SPI_MASTER_OSC_DIV128 = 0b01010110, // Maestro, reloj dividido por 128
	SPI_SLAVE_SS          = 0b01000000  // Esclavo, señal de selección de esclavo
} Spi_Type;

// Orden de bits de datos en SPI
typedef enum {
	SPI_DATA_ORDER_MSB = 0b00000000, // MSB primero
	SPI_DATA_ORDER_LSB = 0b00100000  // LSB primero
} Spi_Data_Order;

// Polaridad del reloj SPI
typedef enum {
	SPI_CLOCK_IDLE_HIGH = 0b00001000, // Reloj en alto cuando está inactivo
	SPI_CLOCK_IDLE_LOW  = 0b00000000  // Reloj en bajo cuando está inactivo
} Spi_Clock_Polarity;

// Fase del reloj SPI
typedef enum {
	SPI_CLOCK_FIRST_EDGE = 0b00000000, // Muestra los datos en el primer flanco
	SPI_CLOCK_LAST_EDGE  = 0b00000100  // Muestra los datos en el último flanco
} Spi_Clock_Phase;

// Prototipos de funciones para la configuración y uso de SPI
void SpiInit(Spi_Type, Spi_Data_Order, Spi_Clock_Polarity, Spi_Clock_Phase);
void SpiWrite(uint8_t dat);
unsigned SpiDataReady();
uint8_t SpiRead(void);

#endif /* SPI_M_H_ */
