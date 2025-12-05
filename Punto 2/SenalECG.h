#pragma once
#include <string>
#include <vector>

struct MuestraECG {
    double tiempo;      // segundos
    double amplitud;    // mV o unidades de entrada
    double filtrada;    // salida del filtro (se calcula luego)
    MuestraECG* prev;
    MuestraECG* next;
    MuestraECG(double t, double a)
        : tiempo(t), amplitud(a), filtrada(0.0), prev(nullptr), next(nullptr) {}
};

class SenalECG {
public:
    SenalECG();
    ~SenalECG();

    bool cargarDesdeArchivo(const std::string& rutaArchivo);

    // Filtro pasa bajos IIR de 1er orden: y[n] = alpha*y[n-1] + (1-alpha)*x[n]
    // cutoffHz: frecuencia de corte en Hz (p.ej. 15 Hz para suavizado ECG)
    // Si dt no se provee, se estima del archivo como promedio de diferencias.
    void filtrarPasaBajos(double cutoffHz, double dt = -1.0);

    // Detecta picos (R) sobre la señal filtrada usando umbral y vecindario.
    // umbral: mínimo valor para considerar pico.
    // ventana: número de muestras alrededor para condición de máximo local.
    // retorna tiempos de picos detectados.
    std::vector<double> detectarPicos(double umbral, int ventana);

    // Escribe original (x,t) y filtrada (y,t) a archivo de salida.
    bool imprimirSenales(const std::string& rutaSalida);

    // Calcula frecuencia cardiaca promedio (BPM) basada en tiempos de picos.
    double frecuenciaCardiacaPromedio(const std::vector<double>& tiemposPicos) const;

    // Utilidad: número de muestras, primer/último tiempo.
    size_t size() const;
    double primerTiempo() const;
    double ultimoTiempo() const;

private:
    MuestraECG* head;
    MuestraECG* tail;
    size_t n;

    void limpiar();
    double estimarDt() const;
};
