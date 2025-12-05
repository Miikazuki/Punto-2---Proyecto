# Proyecto ECG en C++ (Lista doble)

Este proyecto implementa una clase `SenalECG` en C++ que almacena una señal ECG en una lista enlazada doble y provee:

- Carga desde archivo de texto (`ECG.txt`) con líneas `amplitud tiempo` (o `tiempo amplitud`).
- Carga desde archivo de texto (`ECG.txt`) con líneas `amplitud tiempo`, `tiempo amplitud` o CSV `value,time` (con encabezado opcional).
- Filtro digital pasa bajos IIR de primer orden.
- Detección de picos (máximos locales) en la señal filtrada.
- Exportación de señal original y filtrada a `ECG_filtrada.txt`.
- Cálculo de frecuencia cardiaca promedio (BPM) a partir de los picos.

## Estructura
- `SenalECG.h` / `SenalECG.cpp`: implementación de la clase y la lista doble.
- `main.cpp`: ejecutable de ejemplo.
- `Punto1.cpp`: ejercicio adicional de lista enlazada simple para estudiantes (`ListadoEstudiantil`).
- `Punto 3\Punto3.cpp`: simulación simple de dominó con historial enlazado.

## Compilación en Windows (PowerShell)

### Con g++ (MinGW)
```powershell
g++ -std=c++17 "c:\Users\danre\Desktop\Punto 2 - Proyecto\main.cpp" "c:\Users\danre\Desktop\Punto 2 - Proyecto\SenalECG.cpp" -o "c:\Users\danre\Desktop\Punto 2 - Proyecto\ecg.exe"
```

### Con MSVC (cl)
```powershell
cl /EHsc /std:c++17 "c:\Users\danre\Desktop\Punto 2 - Proyecto\main.cpp" "c:\Users\danre\Desktop\Punto 2 - Proyecto\SenalECG.cpp" /Fe:"c:\Users\danre\Desktop\Punto 2 - Proyecto\ecg.exe"
```

## Ejecución
Por defecto, el programa usa la ruta:
`C:\Users\danre\Desktop\Punto 2 - Proyecto\SenalECG.txt`.
Puedes pasar otra ruta como primer argumento si lo deseas.

```powershell
# Asumiendo ECG.txt en la misma carpeta
"c:\Users\danre\Desktop\Punto 2 - Proyecto\ecg.exe"

# O indicando ruta de archivo
"c:\Users\danre\Desktop\Punto 2 - Proyecto\ecg.exe" "c:\Users\danre\Desktop\Punto 2 - Proyecto\SenalECG.txt"
## Punto 3: Juego de Dominó
- Archivo: `Punto 3\Punto3.cpp`.
- Implementa:
	- `Ficha`: pieza de dominó con representación `[a|b]`.
	- `Historial`: lista enlazada simple que guarda jugador, ficha y estado del tablero tras cada jugada; se imprime a archivo.
	- `Domino`: reparte fichas, alterna turnos entre dos jugadores, actualiza tablero y registra movimientos.
	- `main`: instancia `Domino` y ejecuta `jugar()`.

### Compilar y ejecutar
```powershell
g++ -std=c++17 "c:\Users\danre\Desktop\Punto 2 - Proyecto\Punto 3\Punto3.cpp" -o "c:\Users\danre\Desktop\Punto 2 - Proyecto\Punto 3\punto3.exe"
"c:\Users\danre\Desktop\Punto 2 - Proyecto\Punto 3\punto3.exe"
```

Genera: `historial_domino.txt` con el detalle de las jugadas y el estado del tablero.
## Punto 1: Listado Estudiantil
- Archivo: `Punto1.cpp`.
- Contiene una implementación de lista enlazada simple con las operaciones:
	- Insertar al final (`insertarFinal`).
	- Buscar por identificación (`buscar`).
	- Eliminar por identificación (`eliminar`).
	- Indexar con `operator[]`.
	- Invertir la lista (`invertir`).
	- Imprimir a archivo (`imprimirArchivo`).

### Compilar y ejecutar (ejemplo simple)
```powershell
g++ -std=c++17 "c:\Users\danre\Desktop\Punto 2 - Proyecto\Punto1.cpp" -o "c:\Users\danre\Desktop\Punto 2 - Proyecto\punto1.exe"
"c:\Users\danre\Desktop\Punto 2 - Proyecto\punto1.exe"
```

Nota: `Punto1.cpp` define la clase y estructuras; si necesitas una función `main` de prueba, dime y la agrego para cargar algunos estudiantes y generar un `Listado.txt`.
```

Se generarán `ECG_filtrada.txt` (amplitud_original, filtrada, tiempo) y `picos.txt` (tiempos de picos).

## Parámetros y calibración
- Filtro: ajuste `cutoffHz` en `main.cpp` (por defecto 15 Hz). Si el muestreo no está en el archivo, se estima el `dt` automáticamente, con fallback a 250 Hz.
- Picos: ajuste `umbral` y `ventana` en `main.cpp`. Un buen inicio es poner el umbral a ~40% del máximo filtrado y ventana entre 6-12 muestras.

## Formato de entrada
`ECG.txt` puede ser texto separando por espacio o CSV.

Ejemplo texto (espacio):
```
0.12 0.0000
0.15 0.0040
...
```
(Amplitud y tiempo en segundos). También se admite `tiempo amplitud`.

Ejemplo CSV (coma) con encabezado:
```
value,time
4.000566,0
7.391435,0.005
...
```
