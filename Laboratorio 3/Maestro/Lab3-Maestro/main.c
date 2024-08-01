#define F_CPU 16000000UL // Define la frecuencia de la CPU para c�lculos de tiempo

#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "SPI-Maestro/SPI-Maestro.h"
#include "USART/USART.h"

// Variables globales
volatile uint8_t valorSpi1, valorSpi2; // Valores le�dos desde SPI
volatile char buffer[16]; // Buffer para almacenar datos formateados
int valorSpi1b, valorSpi2b; // Variables para almacenar valores convertidos
uint8_t inicio = 1; // Indicador de inicio del programa
int received_value = 0; // Valor recibido desde UART
char input_buffer[4]; // Buffer para entrada de datos desde UART

// Configura los puertos, SPI y UART
void setUp() {
	// Configurar pines como salida
	DDRC |= (1 << DDC4) | (1 << DDC5);
	DDRD |= (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
	// Inicializar los pines a bajo
	PORTC &= ~((1 << PORTC4) | (1 << PORTC5));
	PORTD &= ~((1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7));
	// Inicializar SPI y UART
	SpiInit(SPI_MASTER_OSC_DIV128, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_LOW, SPI_CLOK_FIRST_EDGE);
	UART_init(9600);
}

// Env�a los valores le�dos del SPI a trav�s de UART
void enviarValoresUART() {
	valorSpi2b = (int)(valorSpi2);
	valorSpi1b = (int)(valorSpi1);
	UART_send_string("\n\r");
	snprintf(buffer, sizeof(buffer), "P1: %d \r\n", valorSpi1b);
	UART_send_string(buffer);
	_delay_ms(1);
	snprintf(buffer, sizeof(buffer), "P2: %d \r\n", valorSpi2b);
	UART_send_string(buffer);
}

// Realiza la lectura de datos desde el ADC a trav�s de SPI
void lecturaADC() {
	PORTB &= ~(1 << PORTB2); // Selecciona el esclavo SPI
	SpiWrite('c');
	valorSpi1 = SpiRead();
	_delay_ms(10);
	SpiWrite('d');
	valorSpi2 = SpiRead();
	_delay_ms(10);
	PORTB |= (1 << PORTB2); // Desselecciona el esclavo SPI
}

// Funci�n de manejo de interrupciones de comunicaci�n UART
void IntCont() {
	if (UART_available()) {
		char comando = UART_receive();
		switch (comando) {
			case 'm':
			enviarValoresUART();
			_delay_ms(100);
			UART_send_string("\nIngrese 'm' para observar el ADC.\r\n");
			UART_send_string("Ingrese 'n' para el contador.\r\n");
			IntCont();
			break;
			case 'n':
			contNUm();
			_delay_ms(100);
			UART_send_string("\nIngrese 'm' para observar el ADC.\r\n");
			UART_send_string("Ingrese 'n' para el contador.\r\n");
			IntCont();
			break;
			default:
			UART_send_string("\nEl comando ingresado no se reconoce\n");
			break;
		}
	}
}

// Refresca los puertos con el valor dado
void refreshPort(uint8_t value) {
	PORTC = (PORTC & ~((1 << PORTC4) | (1 << PORTC5))) | ((value & 0b00000011) << PORTC4);
	PORTD = (PORTD & ~((1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7))) | ((value & 0b11111100) << (PORTD2 - 2));
}

// Pide un n�mero al usuario y lo muestra en los puertos
void contNUm() {
	UART_send_string("\n\rPor favor, ingresa un n�mero (0-255): ");
	UART_receive_string(input_buffer, sizeof(input_buffer));
	UART_send_string("\nCadena recibida: ");
	UART_send_string(input_buffer);
	received_value = atoi(input_buffer);
	UART_send_string("\nValor recibido como entero: ");
	UART_send_number(received_value);
	if (received_value >= 0 && received_value <= 255) {
		refreshPort(received_value);
		} else {
		UART_send_string("\nValor no v�lido. Por favor, ingresa un n�mero entre 0 y 255.\n");
	}
}

// Funci�n principal del programa
int main(void) {
	setUp();
	sei(); // Habilita interrupciones globales
	while (1) {
		if (inicio == 1) {
			_delay_ms(100);
			UART_send_string("\nIngrese 'm' para observar el ADC.\r\n");
			UART_send_string("Ingrese 'n' para el contador.\r\n");
			lecturaADC();
			_delay_ms(300);
			IntCont();
			inicio = 0;
			} else {
			_delay_ms(500);
			IntCont();
		}
		lecturaADC();
		_delay_ms(300);
	}
}