# SED Pre-processing Clone (Open Source)

Replica abierta del flujo en `examples/pre_processing_SED` con C puro:
- STFT con ventana de Hann (`4096`) y hop (`2048`)
- Magnitud espectral
- Mel filterbank (`40` bandas)
- Compresion log con clip inferior en `-8.0`
- Cuantizacion a `int8`

## Build
```bash
cd test/sed_clone_open
make clean
make all
make test
```

## Uso
```bash
./build/sed_clone_open /ruta/audio_mono16.wav
./build/sed_clone_open --synth
```

## Diferencias vs pipeline HiFi5
- Este proyecto evita dependencias propietarias (NatureDSP/HiFi intrinsics).
- FFT implementada en C (Cooley-Tukey), portable y suficiente para estudio.
- Mel weights se generan en runtime (en HiFi venian precomputados en Q30).

## Puntos para entrevista
1. Arquitectura por bloques: STFT -> mag -> mel -> log -> quant.
2. Tradeoff: claridad/portabilidad vs maximo rendimiento SIMD/ISA especifica.
3. Paso a produccion: fixed-point, profiling por frame, buffers estaticos y pruebas con vectores golden.
