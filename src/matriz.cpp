#include <iostream>
#include "matriz.h"

using namespace std;

Matriz::Matriz() {
    matriz = NULL;
    fils = 0;
    cols = 0;
}

Matriz::Matriz(int filas, int colms) :
fils(filas), cols(colms) {
    // cout << "entrando";
    matriz = new int[filas * colms];

    for (int i = 0; i < fils * cols; i++)
        matriz[i] = 0;
}

Matriz::Matriz(const Matriz& m) {
    matriz = new int[m.fils * m.cols];
    fils = m.fils;
    cols = m.cols;

    for (int i = 0; i < fils * cols; i++)
        matriz[i] = m.matriz[i];
}

Matriz& Matriz::operator=(const Matriz& m) {
    if (&m != this) {
        delete[] matriz;
        fils = m.fils;
        cols = m.cols;
        matriz = new int[fils * cols];

        for (int i = 0; i < fils * cols; i++)
            matriz[i] = m.matriz[i];
    }
    return *this;
}

bool Matriz::operator==(const Matriz &m) const{
    for (int i = 0; i < fils; i++)
        for (int j = 0; j < cols; j++)
            if (getElemento(i,j) != m.getElemento(i,j))
                return false;
    return true;
}

istream& operator>>(istream &flujo, Matriz& m) {

    flujo >> m.fils;
    flujo >> m.cols;
    m.matriz = new int[m.fils * m.cols];

    for (int i = 0; i < m.fils * m.cols; i++)
        flujo >> m.matriz[i];

    return flujo;
}

ostream& operator<<(ostream &flujo, const Matriz& m) {
    flujo << m.fils << ' ' << m.cols << '\n';

    for (int i = 0; i < m.fils * m.cols; i++) {
        flujo << m.matriz[i];
        if (i != 0 && (i + 1) % (m.cols) == 0)
            flujo << '\n';
        else
            flujo << ' ';
    }

    return flujo;
}
