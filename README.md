--- VERSION EN DESARROLLO ---

Libreria de Software Timers pensada para microcontroladores de bajos y medios recursos

- timers_init() - recibe el array de timers timer_t y el numero de timers creados

- give_timer() - asigna el callback al soft timer. Devuelve numero de timer asociado al callback
    Utilizar variables globales o visibles por los callbacks para guardar los numeros de timer

- timers_tick() - Colocar en ISR de timer segun arquitectura

- timers_process(prioridad) - llama callbacks de timers vencidos
    Colocar en loop de ejecucion segun prioridad
    Colocar en ISR solo si la funcion es acotada - usar con criterio

timers_config.h - configuraciones de funcionalidad

--- IDEAS A FUTURO ---

- timers_process(prioridad) -> atender a todos los callbacks de prioridad mayor o igual a la que se le pasa
	--- Actualmente el manejo de prioridades depende del orden de escritura por parte del usuario ---

- Definir una estructura que guarde los return de los callbacks, o que se pueda configurar el return type

- Utilizar el bit reservado como flag de software watchdog, implementando un contador watchdog del callback
	Orientado para callbacks que requieran un tiempo de ejecucion especifico
	Implementacion opcional mediante define en timers_config.h

--- SECCION PORTS EN DESARROLLO ---