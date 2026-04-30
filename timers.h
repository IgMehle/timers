/**
 * timers.h
 *
 *  Created on: 1 feb. 2026
 *      Author: Ignacio Mehle
 */

#ifndef TIMERS_H_
#define TIMERS_H_

/* ===== INCLUDES ===== */
#include "timers_config.h"
#include "portable/timers_port_select.h"

/* ===== DEFINICIONES ===== */
#define EVENT_PENDING   (1)
#define EVENT_CLEAR     (0)
#define MAX_PRIORITIES  (8)
#define PERIODIC_TIMER	(0xFF)
#define TIMER_MAX_REP   (0xFE)
#define CALLBACK_OK     (0)
#define QUEUE_NACK      (0)
#define QUEUE_ACK       (1)
#define TIMER_CRITICAL  (0xFF)

/* ===== COUNTERS SIZE ===== */
#if USE_16BIT_COUNTERS
typedef uint16_t    counter_t  
#else
typedef uint32_t    counter_t
#endif

/* ===== TIMER BYTE FLAGS ===== */
// 0: enabled
// 1: pending (event)
// 2: critical
// 3: prescaler
// 4: RESERVED (watchdog_expired)
// 5: priority.b0
// 6: priority.b1
// 7: priority.b2
#define FLAG_ENABLED    (1 << 0)
#define FLAG_PENDING    (1 << 1)
#define FLAG_CRITICAL   (1 << 2)
#define FLAG_PRESCALER  (1 << 3)
// #define FLAG_WD_EXPIRED (1 << 4)
#define PRIORITY_MASK   (0xE0)
#define PRIORITY_SHIFT  (5)

/* ===== FLAGS INLINES ===== */
static inline uint8_t flags_get(uint8_t flags, uint8_t mask) {
    return (flags & mask);
}
static inline void flags_set(uint8_t *flags, uint8_t mask) {
    *flags |= mask;
}
static inline void flags_clear(uint8_t *flags, uint8_t mask) {
    *flags &= ~mask;
}
static inline uint8_t flags_get_priority(uint8_t flags) {
    return (flags & PRIORITY_MASK) >> PRIORITY_SHIFT;
}
static inline void flags_set_priority(uint8_t *flags, uint8_t priority) {
    priority &= 0x07;
    *flags &= ~PRIORITY_MASK;
    *flags |= (priority << PRIORITY_SHIFT);
}

/**
 * @brief Puntero a funcion callback del timer.
 * Firma: TIMER_CALLBACK_RET_TYPE callback(void *context) si USE_CALLBACK_CONTEXT,
 *        TIMER_CALLBACK_RET_TYPE callback(void)           si no.
 */
typedef TIMER_CALLBACK_RET_TYPE (*timer_callback_t)(TIMER_CALLBACK_ARGS);

/* ===== TIMER_T ===== */
typedef struct {
    volatile uint8_t    flags;
//    uint8_t             priority;
//	volatile uint8_t    enabled;
//#if USE_QUEUES == 0
//	volatile uint8_t    event;
//#endif
    volatile uint8_t    rep;
    counter_t           reload;
    volatile counter_t  ticks;
    timer_callback_t    callback;
    /**
     * @brief Puntero opaco al estado del llamador, pasado automaticamente al callback.
     *
     * El tipo real lo conoce el usuario de la libreria. Dentro del callback,
     * castearlo al tipo correcto:
     *
     *   my_type_t *ctx = (my_type_t *)context;
     *
     * IMPORTANTE: apuntar siempre a memoria de vida util suficiente (static o global).
     * Una variable local puede no existir al momento de la ejecucion del callback.
     */
#if USE_CALLBACK_CONTEXT
    void                *context;
#endif
} timer_t;

// Macro de llamada al callback
#if USE_CALLBACK_CONTEXT
    #define TIMER_INVOKE_CALLBACK(timer) (timer)->callback((timer)->context)
#else
    #define TIMER_INVOKE_CALLBACK(timer) (timer)->callback()
#endif

// Argumentos de give_timer()
#if USE_CALLBACK_CONTEXT
    #define GIVE_TIMER_ARGS timer_callback_t callback, void *context
#else
    #define GIVE_TIMER_ARGS timer_callback_t callback
#endif

/* ===== TIMERS_QUEUE_T ===== */
typedef struct {
    volatile uint8_t    bf[TIMERS_QUEUE_SIZE];
    // escribe la ISR (timers_tick)
    volatile uint8_t    head;
    // lee el consumidor (timers_process)
    volatile uint8_t    tail;
    // flags
    // volatile uint8_t empty;
    // volatile uint8_t full;
} timers_queue_t;

/* ===== INIT / CONFIG ===== */

/**
 * @brief Apunta el puntero general de timers al array de usuario
 * 
 * @param my_timers Puntero al array de timers
 * @param n         Tamaño del array
 */
void timers_init(timer_t *my_timers, uint8_t n);

/**
 * @brief Registra un nuevo timer
 *
 * @param ticks     Valor inicial y de recarga del contador (en ticks)
 * @param priority  Prioridad del timer (0 = maxima)
 * @param callback  Funcion a ejecutar al vencer el timer
 * @param context   (Solo si USE_CALLBACK_CONTEXT) Puntero al estado que se
 *                  pasara al callback. Debe apuntar a memoria estatica o global.
 * @return ID del timer asignado, o 0xFF si no hay mas espacio para asignar
 */
uint8_t give_timer(counter_t ticks, uint8_t priority, GIVE_TIMER_ARGS);

/* ===== TIMER MANIPULATION ===== */

/**
 * @brief Habilita el timer
 * 
 * @param id    ID del timer
 * @param rep   Cantidad de repeticiones
 *              PERIODIC_TIMER: Timer periodico
 */
void on_timer(uint8_t id, uint8_t rep);

/**
 * @brief Detiene el timer
 * 
 * @param id    ID del timer
 */
void pause_timer(uint8_t id);

/**
 * @brief Vuelve a habilitar el timer sin reiniciarlo
 * 
 * @param id    ID del timer
 */
void continue_timer(uint8_t id);

/**
 * @brief Recarga el timer
 * 
 * @param id    ID del timer
 */
void reload_timer(uint8_t id);

/**
 * @brief Setea prioridad del timer
 * 
 * @param id        ID del timer
 * @param priority  Prioridad del timer
 *                  MAX: 0
 *                  MIN: 7
 *                  TIMER_CRITICAL: Si (USE_TIMER_CRITICAL) esta activo
 *                  habilita el FLAG_CRITICAL, si no esta activo,
 *                  asigna maxima prioridad al timer (0)
 */
void set_timer_priority(uint8_t id, uint8_t priority);

/**
 * @brief Setea la cantidad de repeticiones del timer
 * 
 * @param id    ID del timer
 * @param rep   Cantidad de repeticiones, al finalizar la cuenta el timer se deshabilita
 *              MAX: 254
 *              Pasar 255 o PERIODIC_TIMER convierte el timer en periodico
 */
void set_timer_repeats(uint8_t id, uint8_t rep);

/**
 * @brief Suma repeticiones a las actuales del timer
 * 
 * @param id    ID del timer
 * @param rep   Cantidad de repeticiones que se van a sumar a las actuales
 *              Pensado para timers NO periodicos
 *              Si la cuenta da un overflow, se fija el numero de repeticiones en 254
 */
void add_timer_repeats(uint8_t id, uint8_t rep);

/**
 * @brief Setea el FLAG de PRESCALER del timer
 * 
 * @param id    ID del timer
 */
void set_timer_prescaler(uint8_t id);

/**
 * @brief Limpia el FLAG de PRESCALER del timer
 * 
 * @param id    ID del timer
 */
void clear_timer_prescaler(uint8_t id);

/**
 * @brief Cambia el valor de reload del timer y lo resetea
 * 
 * @param id    ID del timer
 * @param ticks Nuevo valor de reload (ticks)
 */
void resize_timer(uint8_t id, counter_t ticks);

/**
 * @brief Deshabilita el timer y resetea el valor de reload
 * 
 * @param id    ID del timer
 */
void off_timer(uint8_t id);

/**
 * @brief Get the timer status object
 * 
 * @param id        ID del timer
 * @return timer_t  Copia del estado actual del timer
 */
timer_t get_timer_status(uint8_t id);

/* ===== TIMERS CORE ===== */

/**
 * @brief   SYSTEM TICK
 *          Actualiza estado de timers
 *          Encola timers vencidos para ser procesados
 *          Debe colocarse esta funcion en la ISR de un Hardware Timer
 *          EJ: Systick_Hamdler() (ARM Cortex-M)
 */
void timers_tick(void);

/**
 * @brief Encola ID de timers vencidos en la queue correspondiente a su prioridad
 * 
 * @param id        ID del timer a encolar
 * @param priority  Prioridad del callback y de la queue
 * @return uint8_t  QUEUE_ACK: El ID del timer pudo ser encolado correctamente
 *                  QUEUE_NACK: Queue llena, el timer no pudo ser encolado
 */
uint8_t push_timers_queue(uint8_t id, uint8_t priority);

/**
 * @brief Desencola ID de timers según prioridad
 * 
 * @param id        ID del timer sacado de la queue
 * @param priority  Prioridad de la queue a quitar IDs
 * @return uint8_t  QUEUE_ACK: Pudo desencolarse un ID de la queue
 *                  QUEUE_NACK: Queue vacia
 */
uint8_t pop_timers_queue(uint8_t *id, uint8_t priority);


void timers_process(uint8_t priority);

#endif /* TIMERS_H_ */