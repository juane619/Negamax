#ifndef MATRIZ_H
#define MATRIZ_H
#include <iostream>

using namespace std;

class Matriz {
private:
    int* matriz; //!< Member variable "matriz[20][20]"
    int fils; //!< Member variable "filas"
    int cols; //!< Member variable "columnas"
public:
    //constructor por defecto
    Matriz();

    //Constructor dos parametros
    Matriz(int filas, int columnas);

    //destructor

    ~Matriz() {
        delete[] matriz;
    }

    //Constructor de copia
    Matriz(const Matriz& m);

    //Sobrecarga de operador =
    Matriz& operator=(const Matriz& m);
    
    //Operator ==
    bool operator==(const Matriz &m) const;

    //Sobrecarga de operador >> como funcion amiga
    friend istream& operator>>(istream &flujo, Matriz &m);

    friend ostream& operator<<(ostream &flujo, const Matriz &m);

    /** Access matriz[fila][columna]
     * \return The current value of matriz[fila][columna]
     */
    int getElemento(int fila, int columna) const {
        if ((fila >= 0 && fila < fils) && (columna >= 0 && columna < cols))
            return matriz[fila * cols + columna];
        else return -1;
    }

    /** Access filas
     * \return The current value of filas
     */
    int filas() const {
        return fils;
    }

    /** Access columnas
     * \return The current value of columnas
     */
    int columnas() const {
        return cols;
    }

    void setElemento(int fila, int columna, int val) {
        if ((fila >= 0 && fila < fils) && (columna >= 0 && columna < cols))
            matriz[fila * cols + columna] = val;
    }
};


#endif // MATRIZ_H
