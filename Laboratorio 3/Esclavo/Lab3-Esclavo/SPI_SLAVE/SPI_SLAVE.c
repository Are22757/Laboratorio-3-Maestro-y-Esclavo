#include "SPI_SLAVE.h"
#include <avr/io.h>

// Función auxiliar para configurar la velocidad del SPI
static void SpiSetSpeed(uint8_t speed) {
	switch(speed) {
		case 0: // div2
		SPCR &= ~((1<<SPR1) | (1<<SPR0));
		SPSR |= (1<<SPI2X);
		break;
		case 1: // div4
		SPCR &= ~((1<<SPR1) | (1<<SPR0));
		SPSR &= ~(1<<SPI2X);
		break;
		case 2: // div8
		SPCR |= (1<<SPR0);
		SPCR &= ~(1<<SPR1);
		SPSR |= (1<<SPI2X);
		break;
		case 3: // div16
		SPCR |= (1<<SPR0);
		SPCR &= ~(1<<SPR1);
		SPSR &= ~(1<<SPI2X);
		break;
		case 4: // div32
		SPCR &= ~(1<<SPR0);
		SPCR |= (1<<SPR1);
		SPSR |= (1<<SPI2X);
		break;
		case 5: // div64
		SPCR &= ~(1<<SPR0);
		SPCR |= (1<<SPR1);
		SPSR &= ~(1<<SPI2X);
		break;
		case 6: // div128
		SPCR |= (1<<SPR1) | (1<<SPR0);
		SPSR &= ~(1<<SPI2X);
		break;
	}
}

// Inicializa el SPI como maestro o esclavo con la configuración especificada
void SpiInit(Spi_Type Type, Spi_Data_Order DataOrder, Spi_Clock_Polarity ClockPolarity, Spi_Clock_Phase ClockPhase) {
	if (Type & (1<<MSTR)) { // Modo maestro
		DDRB |= (1<<DDB3) | (1<<DDB5) | (1<<DDB2); // MOSI, SCK, SS como salida
		DDRB &= ~(1<<DDB4); // MISO como entrada
		SPCR |= (1<<MSTR); // Habilita modo maestro
		SpiSetSpeed(Type & 0b00000111); // Configura la velocidad
		} else { // Modo esclavo
		DDRB |= (1<<DDB4); // MISO como salida
		DDRB &= ~((1<<DDB3) | (1<<DDB5) | (1<<DDB2)); // MOSI, SCK, SS como entrada
		SPCR &= ~(1<<MSTR); // Habilita modo esclavo
	}
	
	// Habilita SPI, orden de datos, polaridad de reloj y fase de reloj
	SPCR |= (1<<SPE) | DataOrder | ClockPolarity | ClockPhase;
}

// Escribe un dato en el registro SPI
void SpiWrite(uint8_t dat) {
	SPDR = dat;
}

// Verifica si los datos están listos para leer
unsigned SpiDataReady(void) {
	return (SPSR & (1<<SPIF)) ? 1 : 0;
}

// Lee el dato recibido desde el registro SPI
uint8_t SpiRead(void) {
	while (!(SPSR & (1<<SPIF))); // Espera a que el dato esté listo
	return SPDR;
}
