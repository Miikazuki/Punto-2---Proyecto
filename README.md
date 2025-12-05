# Proyecto C++: Lista y Señal ECG

Este repositorio contiene tres ejercicios organizados en puntos del 1 al 3.

## Punto 1: Listado Estudiantil
- Archivo: `Punto1.cpp`.
- Implementa una lista enlazada simple con operaciones:
  - Insertar al final (`insertarFinal`).
  - Buscar por identificación (`buscar`).
  - Eliminar por identificación (`eliminar`).
  - Indexar con `operator[]`.
  - Invertir la lista (`invertir`).
  - Imprimir a archivo (`imprimirArchivo`).

### Compilar y ejecutar
```powershell
g++ -std=c++17 "c:\Users\danre\Desktop\Punto 2 - Proyecto\Punto1.cpp" -o "c:\Users\danre\Desktop\Punto 2 - Proyecto\punto1.exe"
"c:\Users\danre\Desktop\Punto 2 - Proyecto\punto1.exe"
```

Notas:
- `Punto1.cpp` define la clase y estructuras; si necesitas una función `main` de prueba, puedo agregarla para cargar algunos estudiantes y generar `Listado.txt`.

## Punto 2: Señal ECG en lista doble

Implementa la clase `SenalECG` en C++ que almacena una señal ECG en una lista enlazada doble y provee:

- Carga desde texto con líneas `amplitud tiempo` o `tiempo amplitud`.
- También admite CSV `value,time` (con encabezado opcional).
- Filtro pasa bajos IIR de primer orden.
- Detección de picos (máximos locales) en la señal filtrada.
- Exportación de señal original y filtrada a `ECG_filtrada.txt`.
- Cálculo de frecuencia cardiaca promedio (BPM) a partir de los picos.

### Estructura
- `SenalECG.h` / `SenalECG.cpp`: implementación de la clase y la lista doble.
- `main.cpp`: ejecutable de ejemplo.

### Compilación en Windows (PowerShell)

Con g++ (MinGW):
```powershell
g++ -std=c++17 "c:\Users\danre\Desktop\Punto 2 - Proyecto\main.cpp" "c:\Users\danre\Desktop\Punto 2 - Proyecto\SenalECG.cpp" -o "c:\Users\danre\Desktop\Punto 2 - Proyecto\ecg.exe"
```

Con MSVC (cl):
```powershell
cl /EHsc /std:c++17 "c:\Users\danre\Desktop\Punto 2 - Proyecto\main.cpp" "c:\Users\danre\Desktop\Punto 2 - Proyecto\SenalECG.cpp" /Fe:"c:\Users\danre\Desktop\Punto 2 - Proyecto\ecg.exe"
```

### Ejecución
Por defecto, el programa usa:
`c:\Users\danre\Desktop\Punto 2 - Proyecto\SenalECG.txt`.
Puedes indicar otra ruta como primer argumento.

```powershell
"c:\Users\danre\Desktop\Punto 2 - Proyecto\ecg.exe"
"c:\Users\danre\Desktop\Punto 2 - Proyecto\ecg.exe" "c:\Users\danre\Desktop\Punto 2 - Proyecto\SenalECG.txt"
```

Salida generada:
- `ECG_filtrada.txt` (amplitud_original, filtrada, tiempo)
- `picos.txt` (tiempos de picos)

### Parámetros y calibración
- Filtro: ajustar `cutoffHz` en `main.cpp` (por defecto 15 Hz). Si el muestreo no está en el archivo, se estima `dt` automáticamente, con fallback a 250 Hz.
- Picos: ajustar `umbral` y `ventana` en `main.cpp`. Un buen inicio es umbral ≈ 40% del máximo filtrado y ventana entre 6-12 muestras.

### Formato de entrada
`ECG.txt` puede ser texto (espacios) o CSV.

Ejemplo texto:
```
0.12 0.0000
0.15 0.0040
...
```
(Amplitud y tiempo en segundos). También se admite `tiempo amplitud`.

Ejemplo CSV con encabezado:
```
value,time
4.000566,0
7.391435,0.005
...
```

## Punto 3: Juego de Dominó
- Archivo: `Punto 3\Punto3.cpp`.
- Implementa:
  - `Ficha`: pieza de dominó `[a|b]`.
  - `Historial`: lista enlazada simple que guarda jugador, ficha y estado del tablero tras cada jugada; se imprime a archivo.
  - `Domino`: reparte fichas, alterna turnos entre dos jugadores, actualiza tablero y registra movimientos.
  - `main`: instancia `Domino` y ejecuta `jugar()`.

### Compilar y ejecutar
```powershell
g++ -std=c++17 "c:\Users\danre\Desktop\Punto 2 - Proyecto\Punto 3\Punto3.cpp" -o "c:\Users\danre\Desktop\Punto 2 - Proyecto\Punto 3\punto3.exe"
"c:\Users\danre\Desktop\Punto 2 - Proyecto\Punto 3\punto3.exe"
```

Salida generada:
- `historial_domino.txt`: detalle de jugadas y estado del tablero.
