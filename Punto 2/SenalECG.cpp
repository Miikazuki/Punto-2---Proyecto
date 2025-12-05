#include "SenalECG.h"
#include <fstream>
#include <sstream>
#include <cmath>

SenalECG::SenalECG() : head(nullptr), tail(nullptr), n(0) {}

SenalECG::~SenalECG() { limpiar(); }

void SenalECG::limpiar() {
    MuestraECG* cur = head;
    while (cur) {
        MuestraECG* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    head = tail = nullptr;
    n = 0;
}

size_t SenalECG::size() const { return n; }

double SenalECG::primerTiempo() const { return head ? head->tiempo : 0.0; }

double SenalECG::ultimoTiempo() const { return tail ? tail->tiempo : 0.0; }

bool SenalECG::cargarDesdeArchivo(const std::string& rutaArchivo) {
    limpiar();
    std::ifstream in(rutaArchivo);
    if (!in.is_open()) return false;

    std::string line;
    bool primeraLinea = true;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        double a = 0.0, t = 0.0;
        // Detectar formato CSV con coma
        if (line.find(',') != std::string::npos) {
            // parsear "valor,tiempo" o "tiempo,valor"
            std::istringstream iss(line);
            std::string s1, s2;
            if (!std::getline(iss, s1, ',')) continue;
            if (!std::getline(iss, s2, ',')) continue;
            std::istringstream v1(s1), v2(s2);
            if (!(v1 >> a) || !(v2 >> t)) {
                // intentar orden inverso
                v1.clear(); v2.clear(); v1.str(s1); v2.str(s2);
                if (!(v1 >> t) || !(v2 >> a)) continue;
            }
        } else {
            // Formatos "a t" o "t a" separados por espacios
            std::istringstream iss(line);
            if (!(iss >> a >> t)) {
                iss.clear(); iss.str(line);
                if (!(iss >> t >> a)) continue;
            }
        }
        MuestraECG* node = new MuestraECG(t, a);
        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        ++n;
    }
    return n > 0;
}

double SenalECG::estimarDt() const {
    if (n < 2 || !head) return -1.0;
    // Promedio de diferencias de tiempo
    double sum = 0.0;
    size_t cnt = 0;
    for (MuestraECG* cur = head; cur && cur->next; cur = cur->next) {
        double dt = cur->next->tiempo - cur->tiempo;
        if (dt > 0) { sum += dt; ++cnt; }
    }
    return cnt ? (sum / cnt) : -1.0;
}

void SenalECG::filtrarPasaBajos(double cutoffHz, double dt) {
    if (n == 0 || !head) return;
    if (dt <= 0.0) dt = estimarDt();
    if (dt <= 0.0) dt = 1.0 / 250.0; // fallback: 250 Hz

    // Coeficientes de filtro IIR de 1er orden
    // alpha = exp(-2*pi*fc*dt)
    const double pi = std::acos(-1.0);
    const double alpha = std::exp(-2.0 * pi * cutoffHz * dt);
    const double b = 1.0 - alpha; // (1-alpha)

    // Inicialización: salida filtrada igual a primera amplitud
    head->filtrada = head->amplitud;

    for (MuestraECG* cur = head->next; cur; cur = cur->next) {
        // y[n] = alpha*y[n-1] + (1-alpha)*x[n]
        cur->filtrada = alpha * cur->prev->filtrada + b * cur->amplitud;
    }
}

std::vector<double> SenalECG::detectarPicos(double umbral, int ventana) {
    std::vector<double> picos;
    if (n == 0 || !head) return picos;
    if (ventana < 1) ventana = 1;

    // Recorremos evitando extremos para comparar vecindario completo
    for (MuestraECG* cur = head; cur; cur = cur->next) {
        double y = cur->filtrada;
        if (y < umbral) continue;

        bool esMaxLocal = true;
        // mirar hacia atrás
        MuestraECG* left = cur;
        for (int i = 0; i < ventana && left && left->prev; ++i) {
            left = left->prev;
            if (left->filtrada >= y) { esMaxLocal = false; break; }
        }
        if (!esMaxLocal) continue;
        // mirar hacia adelante
        MuestraECG* right = cur;
        for (int i = 0; i < ventana && right && right->next; ++i) {
            right = right->next;
            if (right->filtrada > y) { esMaxLocal = false; break; }
        }
        if (esMaxLocal) {
            picos.push_back(cur->tiempo);
            // saltar algunos vecinos para evitar detectar el mismo complejo R
            for (int i = 0; i < ventana && cur->next; ++i) cur = cur->next;
        }
    }
    return picos;
}

bool SenalECG::imprimirSenales(const std::string& rutaSalida) {
    std::ofstream out(rutaSalida);
    if (!out.is_open()) return false;
    out << "# amplitud_original filtrada tiempo\n";
    for (MuestraECG* cur = head; cur; cur = cur->next) {
        out << cur->amplitud << ' ' << cur->filtrada << ' ' << cur->tiempo << '\n';
    }
    // Además exportar CSV con encabezado para graficar fácilmente
    try {
        std::ofstream csv("ECG_filtrada.csv");
        if (csv.is_open()) {
            csv << "amplitud,filtrada,tiempo\n";
            for (MuestraECG* cur = head; cur; cur = cur->next) {
                csv << cur->amplitud << ',' << cur->filtrada << ',' << cur->tiempo << '\n';
            }
        }
    } catch (...) {
        // ignorar errores de CSV opcional
    }
    return true;
}

double SenalECG::frecuenciaCardiacaPromedio(const std::vector<double>& tiemposPicos) const {
    if (tiemposPicos.size() < 2) return 0.0;
    // RR interval promedio en segundos
    double sumRR = 0.0;
    size_t cnt = 0;
    for (size_t i = 1; i < tiemposPicos.size(); ++i) {
        double rr = tiemposPicos[i] - tiemposPicos[i - 1];
        if (rr > 0) { sumRR += rr; ++cnt; }
    }
    if (cnt == 0) return 0.0;
    double rrProm = sumRR / cnt;
    double bpm = 60.0 / rrProm;
    return bpm;
}
