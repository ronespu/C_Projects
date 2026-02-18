# DSP Embedded Interview Prep (2 dias)

Este paquete te prepara para preguntas practicas del puesto:
- Embedded C (memoria, punteros, errores, debugging)
- DSP audio en plataforma embebida
- Integracion de comando/control en pipeline DSP
- Unit testing y mentalidad de static analysis

## Estructura
- `src/ring_buffer.*`: buffer circular sin malloc (estilo embedded)
- `src/dsp_control.*`: comando/control (gain, bypass, reset)
- `src/sed_extractor.*`: feature extractor SED base (RMS, ZCR, centroid, flux, log-mel)
- `src/demo_main.c`: demo end-to-end
- `tests/*.c`: pruebas unitarias

## Build y ejecucion
```bash
cd test/interview_prep
make clean
make all
```

Para correr con sanitizers:
```bash
make sanitize
```

## Como vender este proyecto en entrevista
1. Explica que evitaste dependencias propietarias y armaste una base portable en C puro.
2. Menciona decisiones de embedded: sin memoria dinamica, validacion de entradas, estados claros.
3. Explica tradeoff clave: DFT O(N^2) sirve para prototipo; en produccion migras a FFT optimizada (CMSIS-DSP/KissFFT/FFTW segun plataforma).
4. Conecta con DSP platform: el bloque `dsp_control` representa command and control integrado con pipeline de audio.

## Preguntas probables + respuesta corta
1. Como manejas memoria en embedded audio?
- Buffers estaticos o pools, sin fragmentacion por malloc/free en runtime, limites definidos por frame-size.

2. Como haces root cause de glitch de audio?
- Reproducir con test vector, medir CPU load/ISR jitter, revisar overruns de buffer, aislar modulo, validar fixed-point scaling/clipping.

3. Que validas con unit tests en DSP?
- Rangos de features, regresion contra golden vectors, estabilidad numerica y manejo de errores.

4. Que revisa static analysis aqui?
- Null pointers, overflow, ramas no cubiertas, conversiones peligrosas, API misuse.

5. Diferencia entre feature extractor prototipo y produccion?
- Prototipo prioriza claridad y validacion de concepto; produccion prioriza complejidad temporal, memoria, SIMD/fixed-point y latencia.

## Plan de estudio (48 horas)
Dia 1 (fundamentos + codigo):
1. Corre `make all` y explica cada modulo en voz alta.
2. Modifica `dsp_control` para agregar comando `CMD_SET_LIMITER`.
3. Agrega test de borde para `sed_extractor_init` (fft invalida, sample rate invalido).

Dia 2 (simulacion entrevista):
1. Implementa version fixed-point Q15 de RMS y ZCR.
2. Mide tiempo por frame (`clock_gettime`) y reporta latencia.
3. Prepara pitch de 3 minutos: problema, arquitectura, tradeoffs, resultados.
