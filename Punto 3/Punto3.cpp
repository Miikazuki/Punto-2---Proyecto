 #include <iostream>
 #include <vector>
 #include <string>
 #include <algorithm>
 #include <random>
 #include <ctime>
 #include <fstream>

 using namespace std;

 /* ===========================================================
    ESTRUCTURA FICHA
    =========================================================== */
 struct Ficha {
     int a, b;

     Ficha(int A = 0, int B = 0) : a(A), b(B) {}

     string str() const {
         return "[" + to_string(a) + "|" + to_string(b) + "]";
     }
 };

 /* ===========================================================
    LISTA ENLAZADA PARA HISTORIAL
    =========================================================== */
 struct Movimiento {
     string jugador;
     string ficha;
     string tablero;
     Movimiento* next;

     Movimiento(const string& j, const string& f, const string& t)
         : jugador(j), ficha(f), tablero(t), next(nullptr) {}
 };

 class Historial {
 private:
     Movimiento* head;

 public:
     Historial() : head(nullptr) {}

     void registrar(const string& jugador, const string& ficha, const string& tablero) {
         Movimiento* nuevo = new Movimiento(jugador, ficha, tablero);

         if (!head) {
             head = nuevo;
             return;
         }

         Movimiento* temp = head;
         while (temp->next)
             temp = temp->next;

         temp->next = nuevo;
     }

     void imprimir(const string& archivo) {
         ofstream out(archivo);

         if (!out) {
             cout << "Error al abrir archivo\n";
             return;
         }

         Movimiento* temp = head;
         while (temp) {
             out << "Jugador: " << temp->jugador << "\n";
             out << "Ficha: " << temp->ficha << "\n";
             out << "Tablero: " << temp->tablero << "\n";
             out << "-----------------------------------\n";
             temp = temp->next;
         }
     }

     ~Historial() {
         Movimiento* temp;
         while (head) {
             temp = head;
             head = head->next;
             delete temp;
         }
     }
 };

 /* ===========================================================
    JUEGO DE DOMINÓ
    =========================================================== */
 class Domino {
 private:
     vector<Ficha> jugador1;
     vector<Ficha> jugador2;
     vector<Ficha> tablero;

     Historial historial;

 public:
     Domino() {
         repartirFichas();
     }

     void repartirFichas() {
         vector<Ficha> fichas;

         for (int i = 0; i <= 6; i++)
             for (int j = i; j <= 6; j++)
                 fichas.push_back(Ficha(i, j));

         srand(static_cast<unsigned int>(time(nullptr)));
         // Usar shuffle moderno con generador aleatorio
         std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));
         std::shuffle(fichas.begin(), fichas.end(), rng);

         for (int i = 0; i < 7; i++) jugador1.push_back(fichas[i]);
         for (int i = 7; i < 14; i++) jugador2.push_back(fichas[i]);
     }

     void mostrarTablero() {
         cout << "Tablero: ";
         for (auto& f : tablero) cout << f.str() << " ";
         cout << "\n";
     }

     string tableroToString() {
         string t;
         for (auto& f : tablero) t += f.str() + " ";
         return t;
     }

     void jugar() {
         size_t turno = 0;

         while (!jugador1.empty() || !jugador2.empty()) {

             Ficha f;
             string jugador;

             if (turno % 2 == 0 && !jugador1.empty()) {
                 f = jugador1.back();
                 jugador1.pop_back();
                 jugador = "Jugador 1";
             }
             else if (!jugador2.empty()) {
                 f = jugador2.back();
                 jugador2.pop_back();
                 jugador = "Jugador 2";
             } else break;

             tablero.push_back(f);

             cout << jugador << " juega: " << f.str() << endl;
             mostrarTablero();

             historial.registrar(jugador, f.str(), tableroToString());

             turno++;
         }

         historial.imprimir("historial_domino.txt");
         cout << "\nHistorial guardado en historial_domino.txt\n";
     }
 };

 /* ===========================================================
    FUNCIÓN PRINCIPAL
    =========================================================== */
 int main() {
     Domino game;
     game.jugar();
     return 0;
 }
