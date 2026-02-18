# Interview Advanced Pack

Contenido agregado según la descripción del puesto:

1. Unit testing de borde
- `tests/test_ring_buffer_adv.c`
- `tests/test_diagnostics.c`

2. Static analysis mindset
- `make analyze` ejecuta `gcc -fanalyzer`
- `cppcheck` opcional si está instalado

3. Simulación DMA ping-pong
- `src/pingpong_dma.c` (doble buffer A/B, budget de frame, detección de overrun)

4. Diagnostics + RCA
- `src/diagnostics.*` registra `xrun`, `clip`, `overrun`, pico y tiempo promedio

5. Arquitectura tipo bloques (Simulink/AudioWeaver mindset)
- `src/block_graph_runtime.c`
- parámetros tunables en runtime (`gain`, `preemph`, `bypass`)

## Uso
```bash
cd test/interview_advanced
make clean
make all
make test
make run
make analyze
```

## Cómo usar esto en entrevista
- "Tengo módulos con pruebas de borde y análisis estático para reducir defectos en C embebido".
- "Simulé ping-pong DMA y budget de tiempo por frame para validar latencia y overruns".
- "Implementé diagnóstico para RCA: contadores de clip/xrun/overrun y timing".
- "Separé data path de control con parámetros tunables en runtime, como un graph por bloques".
