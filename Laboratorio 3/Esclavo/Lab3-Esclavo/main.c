#define F_CPU 16000000

#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "ADC/ADC.h"
#include "SPI_SLAVE/SPI_SLAVE.h"

// Variables globales
volatile uint8_t adc_channel = 0;
volatile uint8_t adc_results[2];

// Interrupción del ADC
ISR(ADC_vect) {
	adc_results[adc_channel] = ADC;                 // Almacena el resultado del ADC
	adc_channel ^= 1;                               // Alterna entre 0 y 1
	(adc_channel == 0 ? ADC1 : ADC2)();            // Selecciona el siguiente canal ADC
	iniciar_conversion();                          // Inicia la siguiente conversión
}

int main(void) {
	// Inicializa el SPI en modo esclavo
	SpiInit(SPI_SLAVE_SS, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_LOW, SPI_CLOCK_FIRST_EDGE);

	// Inicializa el ADC
	ADC_init(128);
	iniciar_conversion();   // Inicia la conversión del primer canal
	ADC1();                // Selecciona el primer canal

	// Habilita la interrupción SPI
	SPCR |= (1 << SPIE);
	sei(); // Habilita las interrupciones globales

	while (1) {
		uint8_t spi_data = SPDR; // Lee el dato recibido por SPI
		if (spi_data == 'c' || spi_data == 'd') {
			uint8_t index = (spi_data == 'c') ? 0 : 1; // Determina el índice basado en el comando SPI
			SpiWrite(adc_results[index]);             // Envía el valor ADC correspondiente
			_delay_ms(1);                             // Espera un breve momento
		}
	}
}
