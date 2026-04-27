--- VERSION EN DESARROLLO ---
Libreria de Software Timers pensada para microcontroladores de bajos y medios recursos

timers_init() - recibe el array de timers timer_t y el numero de timers creados
give_timer() - asigna el callback al soft timer. Devuelve numero de timer asociado al callback
    Los callbacks deben "ver" a los numeros de timer que devuelve "give_timer()"
    Utilizar variables globales o visibles por los callbacks
timers_tick() - Colocar en ISR de timer segun arquitectura
timers_process(prioridad) - llama callbacks de timers vencidos
    Colocar en loop de ejecucion segun prioridad
    Colocar en ISR solo si la funcion es acotada - usar con criterio

timers_config.h - configuraciones de funcionalidad

- Ports en desarrollo -