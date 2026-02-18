EMBEDDED DSP INTERVIEW - CHECKLIST FINAL

Fecha de referencia:
- Hoy: 2026-02-18
- Objetivo: entrevista técnica en ~2 días

==================================================
1) COBERTURA CONTRA DESCRIPCION DEL PUESTO
==================================================

[Embedded C]
[ ] Punteros, const, volatile, structs, buffers, stack vs heap
[ ] Explicar por qué evitar malloc/free en runtime crítico
[ ] Manejo defensivo: validación de entradas, límites, saturación

[Debugging / Root Cause Analysis]
[ ] Explicar metodología: reproducir -> instrumentar -> aislar -> corregir -> validar
[ ] Tener 2 casos reales (audio distorsionado / bug intermitente)
[ ] Explicar cómo detectas clipping, overruns, corrupción de memoria

[Memory Management]
[ ] Static allocation y memory pools
[ ] Fragmentación, stack overflow, alignment, determinismo temporal

[DSP Platform - práctico]
[ ] Fixed-point vs float (tradeoffs)
[ ] Latencia por frame, ring buffers, ping-pong buffers, budget por frame
[ ] Separación control plane vs data plane

[Unit Testing + Static Analysis]
[ ] Tests de borde y regresión
[ ] Explicar proceso de análisis estático (MISRA-like mindset)

[Matlab/Simulink]
[ ] Explicar flujo model-based: modelo -> C -> validación contra golden vectors

[AudioWeaver]
[ ] Explicar “block graph”, tuning runtime, integración DSP-C

[Qualcomm 8775/8255]
[ ] Explicar arquitectura high-level: CPU + DSP + IPC + memoria compartida + latencia

==================================================
2) DEMOS DE CODIGO QUE YA TIENES
==================================================

A) Ejemplos base de entrevista:
- test/interview_examples/

B) Suite avanzada (tests, diagnostics, ping-pong, analyze):
- test/interview_advanced/

C) Pipeline SED open-source (clon de pre_processing):
- test/sed_clone_open/

D) Paquete Embedded + DSP + tests inicial:
- test/interview_prep/

==================================================
3) COMANDOS RAPIDOS (PRACTICA)
==================================================

# 1) Ejemplos por tema
cd /mnt/ml_data/iot/audio/Aaron/C_Projects/test/interview_examples
make clean && make all && make run

# 2) Avanzado: tests + run + analyze
cd /mnt/ml_data/iot/audio/Aaron/C_Projects/test/interview_advanced
make clean && make all && make test && make run && make analyze

# 3) SED clon open-source
cd /mnt/ml_data/iot/audio/Aaron/C_Projects/test/sed_clone_open
make clean && make all && make test
./build/sed_clone_open --synth

# 4) Interview prep
cd /mnt/ml_data/iot/audio/Aaron/C_Projects/test/interview_prep
make clean && make all && make test

==================================================
4) PREGUNTAS CLAVE (RESPUESTA CORTA)
==================================================

Q: ¿Por qué evitar malloc/free en embedded audio?
A: No determinismo + fragmentación + riesgo de fallos intermitentes. Prefiero buffers estáticos/pools.

Q: ¿Qué es volatile y cuándo usarlo?
A: En variables compartidas con ISR o registros HW; evita optimizaciones inválidas del compilador.

Q: ¿Cómo reduces latencia en audio?
A: Reduciendo frame size, minimizando copias, optimizando ruta crítica, y vigilando overrun budget.

Q: ¿Cómo investigas audio distorsionado?
A: Verifico input, ganancia, clipping/saturación, scaling fixed-point, y comparo con referencia.

Q: ¿Qué diferencia fixed-point vs floating-point?
A: Fixed: eficiente/determinístico pero exige escala y saturación; float: más simple pero más costo en algunos targets.

Q: ¿Cómo haces RCA de crash intermitente?
A: Reproducibilidad + logs + boundary checks + stack/memoria + aislamiento por módulos + fix + regresión.

==================================================
5) GUIA DE PRESENTACION (3 MINUTOS)
==================================================

1. Problema:
- Integración de pipeline de audio DSP en plataforma embebida con restricciones de latencia y memoria.

2. Solución:
- Módulos C portables con separación control/data path.
- Buffering determinístico, diagnósticos de runtime y pruebas de borde.

3. Validación:
- Unit tests, ejecución reproducible, medición de timing por frame, detección de clipping/overrun.

4. Escalado a producción:
- Migrar kernels críticos a librerías optimizadas/ISA específica (DSP), mantener misma arquitectura de pruebas.

==================================================
6) CHECK FINAL ANTES DE LA ENTREVISTA
==================================================

[ ] Puedo explicar mi pipeline SED de memoria (entrada -> STFT -> mag -> mel -> log -> quant)
[ ] Puedo dibujar arquitectura de audio con control y data path
[ ] Tengo 2 historias STAR de debugging/RCA
[ ] Tengo respuestas cortas para AudioWeaver y Simulink
[ ] Corrí al menos una vez todos los comandos de la sección 3
[ ] Tengo 2-3 preguntas para el entrevistador (arquitectura, tooling, métricas)

