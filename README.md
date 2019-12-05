# Laboratorio Integrador PRAE 2019

## Descripción general

El objetivo del laboratorio es implementar un control de acceso simple.

  - Se debe obtener un código de entrada mediante un teclado matricial.
  - Al ingresar el código correcto se activa un led simulando la apertura de una puerta.
  - Luego de cuatro intentos erróneos se debe generar una melodía y encender cuatro leds en forma alternativa.
  - En todos los casos se informa al usuario del estado mediante un display lcd.


## Organización global del código

Para facilitar el desarrollo sin contar con el kit HCS08 se separaron las funciones dependientes de la arquitectura,
permitiendo depurar los algoritmos utilizando un microcontrolador Atmega328P (Arduino Nano).
Empleando el preprocesador se compila en forma condicional las rutinas de acceso correspondientes.

La temporización de eventos se controla mediante un contador global que se incrementa cada 32us. Cada plataforma
configura un módulo temporizador / RTC y lo actualiza mediante una interrupción.

Los módulos para control de teclado, lcd y generación de melodías se encuentran en la carpeta *lib/*.
El código específico del proyecto se encuentra en *src/*.

Los proyectos para cada plataforma están enlazados a estos archivos.



### Plataforma HCS08

En la carpeta *hcs08/* se encuentra un proyecto para CodeWarrior 6 ya compilado.

Conexionado:

  - Teclado:      Filas en PTB7-4. Columnas en PTB3-0.
  - Parlante:     Cualquier pin de PORTA.
  - LCD_RS:       PTE4
  - LCD_RE:       PTE5
  - LCD_DATA8-4:  PTE3-0
  - Led apertura: PTD0
  - Led alarma:   PTD1



### Plataforma AVR

En la carpeta *avr/* se encuentra un proyecto configurado utilizando [PlatformIO](https://platformio.org/)

Conexionado:

  - Teclado:      Recibe entradas desde la pc por el puerto serie.
  - Parlante:     A0
  - LCD_RS:       D12
  - LCD_RE:       D11
  - LCD_DATA8-4:  D2-5
  - Led apertura: A2
  - Led alarma:   A1



## Módulo LCD

El módulo se inicializa en modo de 4 bits con incremento automático de posición según el protocolo del controlador HD44780.
Define un api básica para mover el cursor y escribir en la pantalla.

```c
void lcd_init_ports(void);
void lcd_reset(void);
void lcd_clear(void);
void lcd_putchar(char c);
void lcd_printf(char const *c);
void lcd_goto(unsigned int line, unsigned int col);
```



## Módulo Teclado

Para la plataforma AVR lee desde el puerto serie.
Para la plataforma HCS08 asume un teclado matricial conectado de la siguiente manera:


    PTB7  -->  1   2   3   A
    PTB6  -->  4   5   6   B
    PTB5  -->  7   8   9   C
    PTB4  -->  *   0   #   D

               v   v   v   v
               |   |   |   |
    PTB3  <----/   |   |   |
    PTB2  <--------/   |   |
    PTB1  <------------/   |
    PTB0  <----------------/


```c
void kbd_init_ports(void);

/*
 *  AVR:   devuelve el último carácter recibido o NO_KEY.
 *
 *  HCS08: devuelve la tecla presionada o NO_KEY.
 *         Para una tecla presionada en forma continua devuelve NO_KEY hasta que no se libere.
 *
 *  Prioridad:
 *  1-9
 *  0
 *  A-D
 *  * #
 */
unsigned char scan_keyboard(void);
```


### Módulo Melodía

Permite reproducir melodías simples. Soporta reproducción continua y reversa.
Precisa que se llame en forma periódica a *music_tick()* para actualizar el estado interno y del parlante.

Interfaz básica:

```c
enum PLAY_STATE {
    STOPPED,
    PAUSED,
    PLAYING,
};

enum PLAY_MODE {
    NORMAL = 0,
    REVERSE = 1,
    LOOP = 2,
};

struct note_info {
    unsigned long note_number;
    unsigned long duration;
};

struct song {
    unsigned long length;
    struct note_info *notes;
};

void music_init_ports(void);

void music_set_song(struct song *target_song);
void music_set_mode(enum PLAY_MODE mode);

void music_play(void);
void music_stop(void);

unsigned char music_is_playing(void);

void music_tick(void);
```

El archivo *music/notes.h* es generado a partir del script *music/notas.py*



## Control de código de acceso

Para verificar que el código ingresado sea correcto se usa una máquina de estados simple implementada
en *src/password.c*. Permite establecer contraseñas de hasta 8 caracteres (es un límite arbitrario).

En el caso de no establecer contraseña cualquier ingreso es considerado válido.

```c
enum PASSWORD_RESULT {
    NONE,
    CORRECT_PASSWORD,
    WRONG_PASSWORD,
};


/*
 * Procesa un caracter y valida la contraseña.
 * Devuelve NONE si aún no tiene datos suficientes
 * Devuelve CORRECT_PASSWORD si la secuencia ingresada es igual a lo establecido con password_set_key()
 * Devuelve WRONG_PASSWORD si la secuencia ingresada NO ES igual a lo establecido con password_set_key()
 */
enum PASSWORD_RESULT password_process_key(char key);

// Solo considera los primeros 8 caracteres
void password_set_key(char const *key);

```



## Bloque de control principal

Los módulos se inicializan desde *src/main.cpp* y en ese archivo se implementa una máquina de estados para simular la
apertura o situación de error (melodía y leds intermitentes) en caso de que se produzcan cuatro intentos erróneos.



# Dificultades encontradas

Debido a que la versión de CodeWarrior empleada posee una limitación al tamaño de código en C++ que permite compilar
todos los módulos y máquinas de estado se implementaron usando C estándar. Si bien el api pública contiene sólo lo
necesario podría haber resultado en código más prolijo.

No obstante eso lograr que el mismo código compile en ambas plataformas no fue tan complicado.
