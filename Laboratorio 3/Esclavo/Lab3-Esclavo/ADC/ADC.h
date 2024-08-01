#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

// Funciones de inicialización
void ADC_init(uint8_t prescaler); // Inicializa el ADC con el prescaler especificado

// Funciones para seleccionar canales ADC
void ADC_select_channel(uint8_t channel); // Selecciona el canal ADC específico
void ADC0(void); // Selecciona el canal 0
void ADC1(void); // Selecciona el canal 1
void ADC2(void); // Selecciona el canal 2
void ADC3(void); // Selecciona el canal 3
void ADC4(void); // Selecciona el canal 4
void ADC5(void); // Selecciona el canal 5
void ADC6(void); // Selecciona el canal 6
void ADC7(void); // Selecciona el canal 7

// Funciones de control
void iniciar_conversion(void); // Inicia la conversión ADC

#endif /* ADC_H_ */
