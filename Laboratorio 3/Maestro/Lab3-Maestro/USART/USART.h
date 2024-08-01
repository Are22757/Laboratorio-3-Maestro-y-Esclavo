#ifndef UART_H_
#define UART_H_

#include <stdint.h> // Incluye los tipos de datos enteros de tama�o fijo

#define F_CPU 16000000UL // Define la frecuencia del reloj del microcontrolador
#define BAUD_PRESCALE(baud) ((F_CPU / (baud * 16UL)) - 1) // Macro para calcular el prescaler del baud rate

// Declaraci�n de variables globales para el manejo del buffer UART
volatile char uart_buffer[128]; // Buffer circular para datos recibidos por UART
volatile uint8_t uart_head; // �ndice del inicio del buffer
volatile uint8_t uart_tail; // �ndice del final del buffer

// Inicializa el UART con un baud rate especificado
void UART_init(unsigned int baud);

// Env�a un solo car�cter a trav�s de UART
void UART_send(unsigned char data);

// Env�a una cadena de caracteres a trav�s de UART
void UART_send_string(const char* str);

// Env�a un n�mero (como cadena) a trav�s de UART
void UART_send_number(uint8_t number);

// Recibe un solo car�cter desde el buffer UART
unsigned char UART_receive(void);

// Verifica si hay datos disponibles en el buffer UART
int UART_available(void);

// Recibe una cadena desde el buffer UART hasta un delimitador o el final
void UART_receive_string(char* buffer, uint8_t max_length);

#endif /* UART_H_ */
