/**
 * @file main.c
 * @brief Aplicação de teste para controlar um servo motor SG90 diretamente
 * com um sinal PWM gerado por um pino GPIO do Raspberry Pi Pico.
 *
 * Este projeto demonstra como reconfigurar um pino de uma porta I2C
 * para funcionar como uma saída PWM padrão, controlando o ângulo de um servo.
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// --- Configuração do Pino de Controle do Servo ---
// O pino de sinal do servo está conectado ao GPIO2 (SDA da porta I2C 1).
#define SERVO_PIN 2

/**
 * @brief Converte um ângulo (0-180 graus) para o valor de duty cycle do PWM.
 * @param angle O ângulo desejado para o servo.
 * @return O valor do duty cycle (0-65535) para a função pwm_set_gpio_level.
 */
uint16_t angle_to_duty(float angle) {
    // A maioria dos servos opera com um pulso entre ~0.5ms e ~2.5ms.
    // Mapeamos o ângulo de 0-180 graus para essa faixa de pulso.
    float pulse_ms = 0.5f + (angle / 180.0f) * 2.0f;

    // Converte a largura do pulso em milissegundos para um valor de duty cycle.
    // O período total do nosso sinal de 50Hz é 20ms.
    // A fórmula é: (largura_do_pulso_ms / período_total_ms)
    // Para um contador de 16 bits, o valor máximo é 65535.
    // No nosso caso, usamos um wrap de 20000 para facilitar, então o cálculo é:
    // Duty = (pulse_ms / 20ms) * 20000
    return (uint16_t)((pulse_ms / 20.0f) * 20000.0f);
}


int main() {
    stdio_init_all();
    while(!stdio_usb_connected()) {
        sleep_ms(100);
    }

    printf("--- Teste de Servo Motor com PWM Direto do Pico ---\n");
    printf("Controlando o servo no pino GPIO%d\n", SERVO_PIN);

    // --- Configuração do Periférico PWM ---

    // 1. Define a função do pino escolhido como PWM.
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    
    // 2. Descobre qual "fatia" (slice) de hardware PWM controla este pino.
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);

    // 3. Configura os parâmetros do PWM para gerar um sinal de 50Hz.
    pwm_config config = pwm_get_default_config();
    // O clock do sistema é 125MHz. Dividindo por 125, temos um "tick" a cada 1us.
    pwm_config_set_clkdiv(&config, 125.0f);
    // Definimos o período para 20.000 ticks de 1us = 20.000us = 20ms (que é 50Hz).
    pwm_config_set_wrap(&config, 20000);
    
    // 4. Inicia a fatia de PWM com a configuração definida.
    pwm_init(slice_num, &config, true);

    printf("PWM inicializado. Movendo o servo...\n");

    // Loop infinito para mover o servo.
    while (1) {
        // Move o servo de 0 para 180 graus, em passos de 5 graus.
        for (int angle = 0; angle <= 180; angle += 5) {
            printf("Movendo para o angulo: %d\n", angle);
            // Define o nível do pino PWM com base no ângulo convertido.
            pwm_set_gpio_level(SERVO_PIN, angle_to_duty(angle));
            sleep_ms(50);
        }
        sleep_ms(1000); // Pausa no final do curso.

        // Move o servo de volta, de 180 para 0 graus.
        for (int angle = 180; angle >= 0; angle -= 5) {
            printf("Movendo para o angulo: %d\n", angle);
            pwm_set_gpio_level(SERVO_PIN, angle_to_duty(angle));
            sleep_ms(50);
        }
        sleep_ms(1000); // Pausa no início do curso.
    }
    return 0;
}