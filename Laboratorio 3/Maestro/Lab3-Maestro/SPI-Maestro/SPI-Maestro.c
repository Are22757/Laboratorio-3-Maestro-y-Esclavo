#include "SPI-Maestro.h"

// Inicializa el SPI con los parámetros especificados
void SpiInit(Spi_Type Type, Spi_Data_Order DataOrder, Spi_Clock_Polarity ClockPolarity, Spi_Clock_Phase ClockPhase) {
	// Configuración de pines:
	// PB2 = SS (Slave Select)
	// PB3 = MOSI (Master Out Slave In)
	// PB4 = MISO (Master In Slave Out)
	// PB5 = SCK (Serial Clock)
	
	if (Type & (1 << MSTR)) { // Si es modo maestro
		DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2); // Configurar MOSI, SCK, SS como salida
		DDRB &= ~(1 << DDB4); // Configurar MISO como entrada
		SPCR |= (1 << MSTR); // Configurar como maestro
		
		uint8_t clockDiv = Type & 0b00000111;
		switch (clockDiv) {
			case 0: // División de reloj por 2
			SPCR &= ~((1 << SPR1) | (1 << SPR0));
			SPSR |= (1 << SPI2X);
			break;
			
			case 1: // División de reloj por 4
			SPCR &= ~((1 << SPR1) | (1 << SPR0));
			SPSR &= ~(1 << SPI2X);
			break;
			
			case 2: // División de reloj por 8
			SPCR |= (1 << SPR0);
			SPCR &= ~(1 << SPR1);
			SPSR |= (1 << SPI2X);
			break;
			
			case 3: // División de reloj por 16
			SPCR |= (1 << SPR0);
			SPCR &= ~(1 << SPR1);
			SPSR &= ~(1 << SPI2X);
			break;
			
			case 4: // División de reloj por 32
			SPCR &= ~(1 << SPR0);
			SPCR |= (1 << SPR1);
			SPSR |= (1 << SPI2X);
			break;
			
			case 5: // División de reloj por 64
			SPCR &= ~(1 << SPR0);
			SPCR |= (1 << SPR1);
			SPSR &= ~(1 << SPI2X);
			break;
			
			case 6: // División de reloj por 128
			SPCR |= ((1 << SPR1) | (1 << SPR0));
			SPSR &= ~(1 << SPI2X);
			break;
		}
		
		} else { // Si es modo esclavo
		DDRB |= (1 << DDB4); // Configurar MISO como salida
		DDRB &= ~((1 << DDB3) | (1 << DDB5) | (1 << DDB2)); // Configurar MOSI, SCK, SS como entrada
		SPCR &= ~(1 << MSTR); // Configurar como esclavo
	}
	
	// Habilitar SPI, establecer orden de bits, polaridad de reloj y fase de reloj
	SPCR |= (1 << SPE) | DataOrder | ClockPolarity | ClockPhase;
}

// Escribe un byte de datos a través de SPI
void SpiWrite(uint8_t dat) {
	SPDR = dat;
}

// Verifica si los datos están listos para ser leídos
unsigned SpiDataReady() {
	return (SPSR & (1 << SPIF)) ? 1 : 0;
}

// Lee un byte de datos recibido a través de SPI
uint8_t SpiRead(void) {
	while (!(SPSR & (1 << SPIF))); // Esperar hasta que el dato esté listo
	return SPDR;
}
