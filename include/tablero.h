#ifndef TABLERO_H
#define TABLERO_H
#include <iostream>
#include <vector>
#include "matriz.h"

using namespace std;

class Tablero {
private:
    Matriz tablero; //!< Member variable "tablero"
    int fichas_linea;
    int fichas_turno;
    int ficha_turno_actual;
    int turno_actual;
    int posibles_movs;
    int actual_mov;
    int ganador;
    int puntos_partida;
    char last_action1;
    char last_action2;
public:
    Tablero();

    Tablero(int fils, int cols, int fichas_linea, int f_turno);


    friend istream& operator>>(istream& flujo, Tablero& t);
    //friend istream& operator>>(istream& flujo, Tablero& t);

    friend ostream& operator<<(ostream& flujo, const Tablero& t);
    
    /** Operator= 
     * 
     */
    bool operator==(const Tablero &tab) const;

    /** Consultar filas
     * \return The current value of filas
     */
    int filas() const {
        return tablero.filas();
    }

    /** Consultar columnas
     * \return The current value of columnas
     */
    int columnas() const {
        return tablero.columnas();
    }

    /** Consultar fichas a alinear
     * \return The current value of fichas
     */
    int fichasLinea() const {
        return fichas_linea;
    }

    int fichasTurno() const {
        return fichas_turno;
    }

    int fichaTurnoActual() const {
        return ficha_turno_actual;
    }

    /** Consultar contenido posicion
     * \return The current value of tablero[fila][columna]
     */

    int getPosicion(int fil, int col) const {
        return tablero.getElemento(fil, col);
    }

    int posiblesMovs() const {
        return posibles_movs;
    }

    int actualMov() const {
        return actual_mov;
    }

    int getGanador() const {
        return ganador;
    }

    int getPuntos() const {
        return puntos_partida;
    }
    
    int lastAction(int jug){
        if(jug==1)
            return last_action1;
        else
            return last_action2;
    }

    bool hayGanador() const{
        return hayLinea();
    }

    bool hayLinea() const;

    bool hayEmpate() const{
        return actual_mov > posibles_movs || (!hayGanador() && turno_actual == 0);
    }

    bool gameEnd() const{
        return hayGanador() || hayEmpate();
    }

    int turnoActual() const {
        return turno_actual;
    }

    void aumentarMov() {
        actual_mov++;
    }

    void cambiarTurno();

    void setTurno(int t) {
        turno_actual = t;
    }

    bool insertar(char col);
    
    bool insertar2(char col);

    void vaciarTablero();

    void prettyPrint(ostream& sal = cout) const;

    void cuentaPuntos();
   
    vector<Tablero> generateMoves() const;
    
    Tablero nextMove(char &last_acc) const;
    
    int possibleAct() const;
};

#endif // TABLERO_H
