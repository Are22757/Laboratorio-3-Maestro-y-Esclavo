#include "ADC.h"
#include <stdint.h>
#include <avr/io.h>

// Función para inicializar el ADC con un prescaler dado
void ADC_init(uint8_t prescaler) {
	// Configuración del VREF y formato de datos
	ADMUX = (1 << REFS0); // VREF = AVCC, datos alineados a la derecha (10 bits)

	// Habilita el ADC y la interrupción ADC
	ADCSRA = (1 << ADEN) | (1 << ADIE);

	// Configuración del prescaler
	switch (prescaler) {
		case 2:   ADCSRA |= (1 << ADPS0); break;
		case 4:   ADCSRA |= (1 << ADPS1); break;
		case 8:   ADCSRA |= (1 << ADPS1) | (1 << ADPS0); break;
		case 16:  ADCSRA |= (1 << ADPS2); break;
		case 32:  ADCSRA |= (1 << ADPS2) | (1 << ADPS0); break;
		case 64:  ADCSRA |= (1 << ADPS2) | (1 << ADPS1); break;
		case 128: ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); break;
		default:  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); break; // Default a 128
	}
}

// Función para seleccionar el canal del ADC
void ADC_select_channel(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // Mantiene los 4 bits altos y cambia los 4 bits bajos
}

// Funciones para seleccionar canales específicos
void ADC0(void) { ADC_select_channel(0); }
void ADC1(void) { ADC_select_channel(1); }
void ADC2(void) { ADC_select_channel(2); }
void ADC3(void) { ADC_select_channel(3); }
void ADC4(void) { ADC_select_channel(4); }
void ADC5(void) { ADC_select_channel(5); }
void ADC6(void) { ADC_select_channel(6); }
void ADC7(void) { ADC_select_channel(7); }

// Función para iniciar la conversión ADC
void iniciar_conversion(void) {
	ADCSRA |= (1 << ADSC);
}
