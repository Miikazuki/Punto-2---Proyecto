#include <iostream>
#include <vector>
#include <fstream>
#include "SenalECG.h"

int main(int argc, char** argv) {
    // Ruta por defecto dentro de la misma carpeta del proyecto
    std::string ruta = (argc > 1) ? argv[1] : "C:\\Users\\danre\\Desktop\\Punto 2 - Proyecto\\SenalECG.txt";
    SenalECG ecg;

    std::ofstream runlog("run_log.txt");
    runlog << "Ruta de entrada: " << ruta << "\n";
    if (!ecg.cargarDesdeArchivo(ruta)) {
        std::cerr << "No se pudo cargar el archivo: " << ruta << std::endl;
        runlog << "ERROR: no se pudo cargar el archivo" << "\n";
        runlog.close();
        return 1;
    }
    runlog << "Muestras cargadas: " << ecg.size() << "\n";

    // Filtrado: ajustar cutoff según el contenido (p.ej., 15 Hz)
    ecg.filtrarPasaBajos(15.0);
    runlog << "Filtrado aplicado (cutoffHz=15)" << "\n";

    // Detectar picos en señal filtrada: ajustar umbral y ventana
    // Un umbral típico puede calibrarse al 30-50% del máximo filtrado; aquí fijo 0.5
    std::vector<double> picos = ecg.detectarPicos(0.5, 8);
    runlog << "Picos detectados: " << picos.size() << "\n";

    // Guardar señal original y filtrada
    if (!ecg.imprimirSenales("ECG_filtrada.txt")) {
        std::cerr << "No se pudo escribir ECG_filtrada.txt" << std::endl;
        runlog << "ERROR: no se pudo escribir ECG_filtrada.txt" << "\n";
    }
    runlog << "Archivos escritos: ECG_filtrada.txt, ECG_filtrada.csv, picos.txt (si hay picos)" << "\n";

    // Calcular BPM promedio
    double bpm = ecg.frecuenciaCardiacaPromedio(picos);
    std::cout << "Muestras: " << ecg.size() << "\n";
    std::cout << "Tiempo: " << ecg.primerTiempo() << "s a " << ecg.ultimoTiempo() << "s\n";
    std::cout << "Picos detectados: " << picos.size() << "\n";
    std::cout << "Frecuencia cardiaca promedio: " << bpm << " BPM\n";

    // También imprimir tiempos de picos
    if (!picos.empty()) {
        std::ofstream out("picos.txt");
        for (double t : picos) out << t << "\n";
        runlog << "Archivo picos.txt generado" << "\n";
    } else {
        runlog << "Sin picos (picos.txt no generado)" << "\n";
    }

    runlog.close();

    return 0;
}
