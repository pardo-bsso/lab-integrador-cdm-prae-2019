#!/usr/bin/env python3

import math

# (nombre, freq)

notas = [
    ('SILENCE', 0),
    ('C4', 261.6256),
    ('C4_SHARP', 277.1826),
    ('D4', 293.6648),
    ('D4_SHARP', 311.1270),
    ('E4', 329.6276),
    ('F4', 349.2282),
    ('F4_SHARP', 369.9944),
    ('G4', 391.9954),
    ('G4_SHARP', 415.3047),
    ('A4', 440.0000),
    ('A4_SHARP', 466.1638),
    ('B4', 493.8833),
    ('C5', 523.2511),
    ('D5', 587.3296),
]

base_period = 31.25e-6

header = """
#ifndef _NOTES_H
#define _NOTES_H

"""

footer = """
#endif
"""

print(header)

for (nota, freq) in notas:
    if freq:
        modulo = math.ceil(1.0 / (2 * freq * base_period))
    else:
        modulo = 0
    define = f'#define {nota: <11} ({modulo})'
    print(define)



# (nombre, divisor)
duraciones = [
    ('WHOLE', 1),
    ('HALF', 2),
    ('QUARTER', 4),
    ('EIGTH', 8),
    ('SIXTIENTH', 16),
]

multiplier = 1.5
base_duration = multiplier * 1.0 / base_period

for (nombre, divisor) in duraciones:
    duracion = math.ceil(base_duration / divisor)
    define = f'#define {nombre: <11} ({duracion})'
    print(define)

print(footer)
