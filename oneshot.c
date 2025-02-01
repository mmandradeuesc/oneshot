#include "pico/stdlib.h"
#include "hardware/timer.h"
#include <stdio.h>

// Definição dos pinos
#define BLUE_LED 11
#define RED_LED 12
#define GREEN_LED 13
#define BUTTON_PIN 5

// Variáveis globais
volatile bool sequence_running = false;
volatile uint8_t current_state = 0;
volatile uint32_t last_button_press = 0;
const uint32_t DEBOUNCE_DELAY = 200; // 200ms para debounce

// Protótipos das funções
void set_leds_state(uint8_t state);
int64_t alarm_callback(alarm_id_t id, void *user_data);
void button_callback(uint gpio, uint32_t events);

// Função para controlar o estado dos LEDs
void set_leds_state(uint8_t state) {
    switch(state) {
        case 0: // Todos desligados
            gpio_put(BLUE_LED, 0);
            gpio_put(RED_LED, 0);
            gpio_put(GREEN_LED, 0);
            printf("Estado: Todos desligados\n");
            break;
        case 1: // Apenas um LED
            gpio_put(BLUE_LED, 0);
            gpio_put(RED_LED, 0);
            gpio_put(GREEN_LED, 1);
            printf("Estado: Um LED ligado\n");
            break;
        case 2: // Dois LEDs
            gpio_put(BLUE_LED, 0);
            gpio_put(RED_LED, 1);
            gpio_put(GREEN_LED, 1);
            printf("Estado: Dois LEDs ligados\n");
            break;
        case 3: // Todos ligados
            gpio_put(BLUE_LED, 1);
            gpio_put(RED_LED, 1);
            gpio_put(GREEN_LED, 1);
            printf("Estado: Todos ligados\n");
            break;
    }
}

// Callback do alarme
int64_t alarm_callback(alarm_id_t id, void *user_data) {
    if (current_state > 0) {
        current_state--;
        set_leds_state(current_state);
        
        if (current_state > 0) {
            // Agenda próximo alarme se ainda houver LEDs para desligar
            add_alarm_in_ms(3000, alarm_callback, NULL, false);
        } else {
            // Sequência completa
            sequence_running = false;
            printf("Sequência completa - Pronto para novo ciclo\n");
        }
    }
    return 0;
}

// Callback do botão com debounce
void button_callback(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    
    // Verifica debounce
    if (current_time - last_button_press < DEBOUNCE_DELAY) {
        return;
    }
    last_button_press = current_time;

    // Só inicia nova sequência se não estiver executando e o botão for pressionado
    if (!sequence_running && (events & GPIO_IRQ_EDGE_FALL)) {
        printf("Botão pressionado - Iniciando sequência\n");
        sequence_running = true;
        current_state = 3;  // Começa com todos os LEDs ligados
        set_leds_state(current_state);
        
        // Inicia a sequência de alarmes
        add_alarm_in_ms(3000, alarm_callback, NULL, false);
    }
}

int main() {
    stdio_init_all();
    printf("Iniciando o programa...\n");

    // Inicializa os pinos dos LEDs
    gpio_init(BLUE_LED);
    gpio_init(RED_LED);
    gpio_init(GREEN_LED);
    
    // Configura LEDs como saída
    gpio_set_dir(BLUE_LED, GPIO_OUT);
    gpio_set_dir(RED_LED, GPIO_OUT);
    gpio_set_dir(GREEN_LED, GPIO_OUT);

    // Inicializa o botão
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN); // Pull-up no botão
    
    // Configura interrupção do botão (na borda de descida porque usamos pull-up)
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, 
                                     GPIO_IRQ_EDGE_FALL,
                                     true, 
                                     &button_callback);

    // Começa com todos os LEDs desligados
    set_leds_state(0);
    printf("Sistema pronto - Aguardando botão ser pressionado\n");

    // Loop principal
    while (true) {
        // O programa é controlado por interrupções
        sleep_ms(100);
    }

    return 0;
}