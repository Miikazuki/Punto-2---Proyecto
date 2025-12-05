
//"""
//Created on Thu Dec  1 10:52:14 2025

//@author: ANDRES
//"""

#include <iostream>
#include <fstream>
#include <string>
using namespace std;


struct Estudiante {
    string nombreCompleto;
    int anioNacimiento;
    long identificacion;
    double promedio;
    string email;
};

struct Nodo {
    Estudiante data;
    Nodo* siguiente;

    Nodo(const Estudiante& e) : data(e), siguiente(nullptr) {}
};

// -------------------------------------------------------------
// Clase ListadoEstudiantil
// -------------------------------------------------------------
class ListadoEstudiantil {
private:
    Nodo* cabeza;
    int tamano;

public:
    // ---------------------------------------------------------
    // Constructor: inicializa lista vacía
    // ---------------------------------------------------------
    ListadoEstudiantil() : cabeza(nullptr), tamano(0) {}

    // ---------------------------------------------------------
    // Insertar estudiante al final
    // ---------------------------------------------------------
    void insertarFinal(const Estudiante& e) {
        Nodo* nuevo = new Nodo(e);

        if (!cabeza) {
            cabeza = nuevo;
        } else {
            Nodo* actual = cabeza;
            while (actual->siguiente != nullptr)
                actual = actual->siguiente;
            actual->siguiente = nuevo;
        }
        tamano++;
    }

    // ---------------------------------------------------------
    // Buscar por identificación (devuelve posición, o -1 si no está)
    // ---------------------------------------------------------
    int buscar(long id) const {
        Nodo* actual = cabeza;
        int pos = 0;

        while (actual != nullptr) {
            if (actual->data.identificacion == id)
                return pos;

            actual = actual->siguiente;
            pos++;
        }
        return -1;
    }

    // ---------------------------------------------------------
    // Eliminar un estudiante por identificación
    // ---------------------------------------------------------
    bool eliminar(long id) {
        if (!cabeza)
            return false;

        // Caso cabeza
        if (cabeza->data.identificacion == id) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            tamano--;
            return true;
        }

        Nodo* actual = cabeza;
        while (actual->siguiente != nullptr &&
               actual->siguiente->data.identificacion != id) {
            actual = actual->siguiente;
        }

        if (actual->siguiente == nullptr)
            return false; // No encontrado

        Nodo* temp = actual->siguiente;
        actual->siguiente = temp->siguiente;
        delete temp;
        tamano--;
        return true;
    }

    // ---------------------------------------------------------
    // Sobrecarga del operador [] para indexación
    // ---------------------------------------------------------
    Estudiante& operator[](int index) {
        if (index < 0 || index >= tamano)
            throw out_of_range("Índice fuera de rango");

        Nodo* actual = cabeza;
        for (int i = 0; i < index; i++)
            actual = actual->siguiente;

        return actual->data;
    }

    const Estudiante& operator[](int index) const {
        if (index < 0 || index >= tamano)
            throw out_of_range("Índice fuera de rango");

        Nodo* actual = cabeza;
        for (int i = 0; i < index; i++)
            actual = actual->siguiente;

        return actual->data;
    }

    // ---------------------------------------------------------
    // Método para invertir la lista enlazada
    // ---------------------------------------------------------
    void invertir() {
        Nodo* previo = nullptr;
        Nodo* actual = cabeza;
        Nodo* siguiente = nullptr;

        while (actual != nullptr) {
            siguiente = actual->siguiente;
            actual->siguiente = previo;
            previo = actual;
            actual = siguiente;
        }
        cabeza = previo;
    }

    // ---------------------------------------------------------
    // Imprimir todos los estudiantes en archivo .txt
    // ---------------------------------------------------------
    void imprimirArchivo(const string& nombreArchivo) const {
        ofstream archivo(nombreArchivo);

        if (!archivo) {
            cerr << "Error al abrir el archivo." << endl;
            return;
        }

        Nodo* actual = cabeza;
        while (actual != nullptr) {
            archivo << "Nombre: " << actual->data.nombreCompleto << endl;
            archivo << "Año de nacimiento: " << actual->data.anioNacimiento << endl;
            archivo << "Identificación: " << actual->data.identificacion << endl;
            archivo << "Promedio: " << actual->data.promedio << endl;
            archivo << "Email: " << actual->data.email << endl;
            archivo << "-----------------------------------" << endl;

            actual = actual->siguiente;
        }

        archivo.close();
    }

    // ---------------------------------------------------------
    // Destructor: libera memoria
    // ---------------------------------------------------------
    ~ListadoEstudiantil() {
        Nodo* actual = cabeza;

        while (actual != nullptr) {
            Nodo* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }
};

// -------------------------------------------------------------
// Punto 1: Programa de prueba
// -------------------------------------------------------------
int main() {
    ListadoEstudiantil lista;

    Estudiante e1{"Ana Gomez", 2001, 1001, 4.2, "ana@example.com"};
    Estudiante e2{"Luis Perez", 2000, 1002, 3.8, "luis@example.com"};
    Estudiante e3{"Marta Diaz", 1999, 1003, 4.5, "marta@example.com"};

    lista.insertarFinal(e1);
    lista.insertarFinal(e2);
    lista.insertarFinal(e3);

    cout << "Tamano inicial: 3" << endl;
    cout << "Posicion id 1002: " << lista.buscar(1002) << endl;

    // Imprimir a archivo
    lista.imprimirArchivo("Listado.txt");
    cout << "Listado.txt generado" << endl;

    // Invertir y reimprimir
    lista.invertir();
    lista.imprimirArchivo("Listado_invertido.txt");
    cout << "Listado_invertido.txt generado" << endl;

    // Eliminar id 1002
    bool eliminado = lista.eliminar(1002);
    cout << "Eliminado id 1002: " << (eliminado ? "si" : "no") << endl;
    lista.imprimirArchivo("Listado_post_eliminar.txt");
    cout << "Listado_post_eliminar.txt generado" << endl;

    // Acceso por indice
    try {
        Estudiante &ref = lista[0];
        cout << "Primer estudiante tras cambios: " << ref.nombreCompleto << endl;
    } catch (const std::exception &ex) {
        cerr << "Error indexacion: " << ex.what() << endl;
    }

    return 0;
}
