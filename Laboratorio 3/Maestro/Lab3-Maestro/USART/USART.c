#include "USART.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define F_CPU 16000000UL
#define BAUD_PRESCALE(baud) ((F_CPU / (baud * 16UL)) - 1)

volatile char uart_buffer[128]; // Buffer circular para almacenamiento de datos recibidos por UART
volatile uint8_t uart_head = 0; // �ndice del inicio del buffer
volatile uint8_t uart_tail = 0; // �ndice del final del buffer

// Inicializa el UART con el baud rate especificado
void UART_init(unsigned int baud) {
	unsigned int ubrr = BAUD_PRESCALE(baud);
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // Habilitar recepci�n, transmisi�n y la interrupci�n de recepci�n
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Configurar para 8 bits de datos, 1 bit de stop
}

// Env�a un solo car�cter a trav�s de UART
void UART_send(unsigned char data) {
	while (!(UCSR0A & (1 << UDRE0))); // Esperar hasta que el buffer de transmisi�n est� vac�o
	UDR0 = data; // Enviar el car�cter
}

// Env�a una cadena de caracteres a trav�s de UART
void UART_send_string(const char* str) {
	while (*str) {
		UART_send(*str++); // Enviar cada car�cter de la cadena
	}
}

// Env�a un n�mero como cadena a trav�s de UART
void UART_send_number(uint8_t number) {
	char buffer[4]; // Buffer para almacenar hasta 3 d�gitos y el terminador de cadena
	snprintf(buffer, sizeof(buffer), "%u", number); // Convertir el n�mero a cadena
	UART_send_string(buffer); // Enviar la cadena resultante
}

// Lee un solo car�cter del buffer UART
unsigned char UART_receive(void) {
	if (uart_head != uart_tail) { // Verificar si hay datos disponibles
		unsigned char data = uart_buffer[uart_tail]; // Leer el car�cter del buffer
		uart_tail = (uart_tail + 1) % sizeof(uart_buffer); // Actualizar el �ndice del final del buffer
		return data; // Devolver el car�cter recibido
		} else {
		return 0; // No hay datos disponibles
	}
}

// Lee una cadena del buffer UART hasta encontrar un delimitador
void UART_receive_string(char* buffer, uint8_t max_length) {
	uint8_t index = 0;
	char received_char;

	while (index < (max_length - 1)) { // Dejar espacio para el terminador de cadena
		if (UART_available()) {
			received_char = UART_receive();
			if (received_char == '\n' || received_char == '\r') {
				// Fin de la cadena
				break;
			}
			buffer[index++] = received_char;
		}
	}
	buffer[index] = '\0'; // Terminar la cadena con un car�cter nulo
}

// Verifica si hay datos disponibles en el buffer UART
int UART_available(void) {
	return (uart_head != uart_tail); // Retorna 1 si hay datos disponibles, 0 si no
}

// Rutina de interrupci�n de recepci�n de UART
ISR(USART_RX_vect) {
	uart_buffer[uart_head] = UDR0; // Leer el car�cter recibido desde el registro UDR0
	uart_head = (uart_head + 1) % sizeof(uart_buffer); // Actualizar el �ndice del inicio del buffer
	uart_buffer[uart_head] = '\0'; // Terminar la cadena con un car�cter nulo
}
