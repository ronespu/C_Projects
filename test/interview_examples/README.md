# Interview Examples (Embedded DSP)

Ejemplos prácticos enfocados a lo que te pueden preguntar en entrevista técnica.

## Compilar y ejecutar
```bash
cd test/interview_examples
make clean
make all
make run
```

## Mapa de ejemplos
- `src/01_memory_management.c`: static allocation vs heap, pool determinístico.
- `src/02_circular_buffer_audio.c`: buffer circular para streaming.
- `src/03_fixed_point_q15.c`: fixed-point básico (Q15) con saturación.
- `src/04_latency_budget.c`: cálculo de latencia por frame-size.
- `src/05_volatile_isr_sim.c`: `volatile` entre ISR simulada y main loop.
- `src/06_race_condition.c`: race condition vs protección con mutex.
- `src/07_debug_distortion_root_cause.c`: análisis de distorsión por clipping.
- `src/08_command_control_dsp.c`: integración de command/control en pipeline DSP.

## Cómo estudiarlo
1. Corre cada binario y explica en voz alta el problema que resuelve.
2. Cambia parámetros (frame, gain, tamaño de pool) y predice el resultado antes de ejecutar.
3. Relaciona cada ejemplo con una respuesta de entrevista (causa, impacto, mitigación).

## Pitch breve sugerido
"Construí una suite de ejemplos en C para practicar memoria, concurrencia, latencia y DSP aplicado. Priorizo diseño determinístico, análisis de causa raíz y separación entre control y data path de audio, que es exactamente lo que requiere integración en plataformas DSP embebidas."
