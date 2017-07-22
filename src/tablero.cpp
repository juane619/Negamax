#include <iostream>
#include <cstdlib>
#include <cmath>
#include "tablero.h"

using namespace std;

Tablero::Tablero() :
tablero() {
    fichas_linea = 0;
    fichas_turno = 0;
    ficha_turno_actual = 1;
    turno_actual = 1;
    posibles_movs = 0;
    actual_mov = 1;
    ganador = 0; //empae o no finalizada: 0.
    puntos_partida = 0;
    last_action1 = last_action2='a'-1;
}

Tablero::Tablero(int fils, int cols, int f_linea, int f_turno) : tablero(fils, cols) {
    fichas_linea = f_linea;
    fichas_turno = f_turno;
    ficha_turno_actual = 1;
    turno_actual = 1;
    posibles_movs = fils*cols;
    actual_mov = 1;
    ganador = 0; //empate o no finalizada: 0.
    puntos_partida = 0;
    last_action1 = last_action2='a'-1;
}

//Tablero::Tablero(const Tablero& t) {
//    tablero = t.tablero;
//    fichas_linea = t.fichas_linea;
//    fichas_turno = t.fichas_turno;
//    ficha_turno_actual = t.ficha_turno_actual;
//    turno_actual = t.turno_actual;
//    posibles_movs = t.posibles_movs;
//    actual_mov = t.actual_mov;
//    ganador = t.ganador; //empae o no finalizada: 0.
//    puntos_partida = t.puntos_partida;
//}
//
//Matriz& Matriz::operator=(const Matriz& m) {
//    if (&m != this) {
//        delete[] matriz;
//        fils = m.fils;
//        cols = m.cols;
//        matriz = new int[fils * cols];
//
//        for (int i = 0; i < fils * cols; i++)
//            matriz[i] = m.matriz[i];
//    }
//    return *this;
//}

bool Tablero::operator==(const Tablero &tab) const {
    return tablero == tab.tablero;
}

istream& operator>>(istream& flujo, Tablero &t) {
    flujo >> t.tablero;
    flujo >> t.fichas_linea;
    flujo >> t.fichas_turno;
    flujo >> t.ficha_turno_actual;
    t.posibles_movs = t.filas() * t.columnas();
    flujo >> t.actual_mov;
    flujo >> t.turno_actual;
    return flujo;
}

ostream& operator<<(ostream &flujo, const Tablero& t) {
    flujo << t.tablero << '\n' << t.fichas_linea << ' ' << t.fichas_turno << ' ' << t.ficha_turno_actual << ' ' << t.actual_mov << ' ' << t.turno_actual << '\n';

    return flujo;
}

bool Tablero::hayLinea() const {
    bool linea_encontrada = false;

    //EL ALGORITMO NO EMPIEZA A BUSCAR HASTA QUE POR LO MENOS NO SE HAN INTRODUCIDO EL DOBLE MENOS DOS FICHAS(minimo de movimientos para poder ganar) PARA MAYOR EFICIENCIA
    if (actual_mov >= fichas_linea * 2 - 2) {
        bool diagonal_izquierda, diagonal_derecha, vertical, horizontal;
        int filas_tab = filas();
        int cols_tab = columnas();
        int tipo_ficha_actual;
        int aux_fila, aux_col;
        int cont_fichas_lin = 0;

        diagonal_izquierda = diagonal_derecha = vertical = horizontal = false;

        /*
        Para cada una de las posiciones de la matriz en las que haya fichas introducidas, primero comprobaremos si es posible
        buscar en cada una de las direcciones posibles(horiz, vert, diag. izq, diag. der.).

        Una vez comprobado las direcciones en las que puede haber una linea(segun esa posicion), intentamos buscar dicha linea en esa direccion.
         */
        for (int fil = 0; fil < filas_tab; fil++)
            for (int col = 0; col < cols_tab; col++) {
                tipo_ficha_actual = getPosicion(fil, col);

                if (tipo_ficha_actual) { //Solo entraremos aqui si la posicion no es cero(hay alguna ficha introducida)
                    aux_fila = fil;
                    aux_col = col;

                    if (col >= fichas_linea - 1 && fil <= filas_tab - fichas_linea)
                        diagonal_izquierda = true;
                    else if (col <= fichas_linea - 1 && fil <= filas_tab - fichas_linea)
                        diagonal_derecha = true;

                    if (fil <= filas_tab - fichas_linea)
                        vertical = true;

                    if (col <= cols_tab - fichas_linea)
                        horizontal = true;

                    //EMPEZAMOS A BUSCAR UNA POSIBLE LINEA
                    if (diagonal_izquierda) {
                        bool sigue = true;

                        cont_fichas_lin = 0;
                        aux_fila = fil;

                        do {
                            aux_fila = aux_fila + 1;
                            aux_col = aux_col - 1;

                            if (getPosicion(aux_fila, aux_col) != tipo_ficha_actual)
                                sigue = false;
                            cont_fichas_lin++;
                        } while (cont_fichas_lin < fichas_linea - 1 && sigue);

                        if (sigue)
                            linea_encontrada = true;
                    }

                    if (diagonal_derecha && !linea_encontrada) {
                        bool sigue = true;

                        cont_fichas_lin = 0;
                        aux_fila = fil;
                        aux_col = col;

                        do {
                            aux_fila = aux_fila + 1;
                            aux_col = aux_col + 1;

                            if (getPosicion(aux_fila, aux_col) != tipo_ficha_actual)
                                sigue = false;
                            cont_fichas_lin++;
                        } while (cont_fichas_lin < fichas_linea - 1 && sigue);

                        if (sigue)
                            linea_encontrada = true;
                    }

                    if (vertical && !linea_encontrada) {
                        bool sigue = true;

                        cont_fichas_lin = 0;
                        aux_fila = fil;
                        aux_col = col;

                        do {
                            aux_fila = aux_fila + 1;

                            if (getPosicion(aux_fila, aux_col) != tipo_ficha_actual)
                                sigue = false;
                            cont_fichas_lin++;
                        } while (cont_fichas_lin < fichas_linea - 1 && sigue);

                        if (sigue)
                            linea_encontrada = true;
                    }

                    if (horizontal && !linea_encontrada) {
                        bool sigue = true;

                        cont_fichas_lin = 0;
                        aux_fila = fil;
                        aux_col = col;

                        do {
                            aux_col = aux_col + 1;

                            if (getPosicion(aux_fila, aux_col) != tipo_ficha_actual)
                                sigue = false;
                            cont_fichas_lin++;
                        } while (cont_fichas_lin < fichas_linea - 1 && sigue);

                        if (sigue)
                            linea_encontrada = true;
                    }
                }
            }
    }
    return linea_encontrada;
}

void Tablero::cambiarTurno() {
    if (turno_actual == 1)
        turno_actual = 2;
    else if (turno_actual == 2)
        turno_actual = 1;
}

//pre: insertar sea posible.
bool Tablero::insertar2(char col) {
    int colum;

    colum = col - 'a';
    for (int i = tablero.filas() - 1; i >= 0; i--)
        if (tablero.getElemento(i, colum) == 0) {
            tablero.setElemento(i, colum, turno_actual);
            aumentarMov();
            
            if(turno_actual==1)
                last_action1= col;
            else
                last_action2=col;
            
            return true;
        }
    
    
    
    return false;
}

/*
Pintamos el tablero escribiendo los correspondientes s�mbolos para cada ficha segun de que jugador sea. Justo debajo comprobamos de quien es el turno
para indicarlo a la hora de insertar ficha y se comprueba tambien si se ha ganado o empatado ya para indicarlo en ese caso.
 */
void Tablero::prettyPrint(ostream& sal) const {
    char col_ind;

    system("clear");
    sal << endl;

    for (col_ind = 'a'; col_ind < 'a' + tablero.columnas(); col_ind++)
        sal << ' ' << col_ind;

    sal << '\n';
    for (int i = 0; i < tablero.filas(); i++) {
        for (int j = 0; j < tablero.columnas(); j++) {
            sal << "|";
            if (tablero.getElemento(i, j) == 0)
                sal << ' ';
            else if (tablero.getElemento(i, j) == 1)
                sal << 'X';
            else if (tablero.getElemento(i, j) == 2)
                sal << 'O';
        }
        sal << '|' << '\n';
    }

    int tope_iguales = tablero.columnas()*2 + 1;
    for (int i = 0; i < tope_iguales; i++)
        sal << '=';

    sal << endl;

    if (!gameEnd()) {
        if (turno_actual == 1)
            sal << "Turno: jugador 1: (X)" << endl;
        else if (turno_actual == 2)
            sal << "Turno: jugador 2: (O)" << endl;
    } else {
        if (hayGanador())
            sal << "Partida finalizada. Ganador: jugador " << turno_actual << endl;
        else
            sal << "Partida finalizada. Empate." << endl;
    }
}

/*Reseteamos el tablero poniendo todas las posiciones a cero junto con el estado del tablero.

Comprobando si en la partida actual empez� el jugador uno para setear el turno inicial al otro
jugador en la siguiente partida en caso de querer seguir jugando.*/

void Tablero::vaciarTablero() {
    for (int f = 0; f < filas(); f++)
        for (int c = 0; c < columnas(); c++)
            tablero.setElemento(f, c, 0);

    bool empezo_uno = (actual_mov % 2 != 0) && (turno_actual == 1);

    if (empezo_uno)
        turno_actual = 2;
    else
        turno_actual = 1;

    ganador = 0;
    actual_mov = 1;
    puntos_partida = 0;
}

/*Se podr�a haber elegido cualquier otro m�todo para elegir la puntuaci�n a la hora de ganar una partida.

En este caso yo he elegido que en cada partida podr�s obtener cuando ganes como mucho 100 puntos y como poco 30, controlando
esto con funciones matem�ticas b�sicas:
        - Segun el tama�o podras obtener como mucho 20 y como poco 10.
        - Segun el n�mero de movimientos empleados para ganar, como mucho 30 y como poco 10. (depender� de las fichas necesarias para alinear).
        - Seg�n la longitud de linea elegida para ganar una partida, como mucho 50 y como poco 10.
 */
void Tablero::cuentaPuntos() {
    double min_movs = fichas_linea * 2 - 1; //minimos movimientos segun las fichas que tengas que alinear

    //CONTAMOS PUNTOS POR TAMAÑO DE TABLERO con 20 puntos maximo si juegas en un tablero 20x20 o 10 si juegas en un tablero 4x4.
    puntos_partida = puntos_partida + log(posibles_movs)*2.8405 + 2.8236;

    //CONTAMOS PUNTOS POR NUMERO DE MOVIMIENTOS
    const double MIN_PUNTOS = 10;
    const double MAX_PUNTOS = 30;

    //Creamos la funcion lineal que da la salida de la puntuacion segun el movimiento actual(7, el minimo posible(30 puntos) y 400 el maximo posible(10 puntos)).
    double pendiente, ordenada;

    pendiente = (MIN_PUNTOS - MAX_PUNTOS) / (posibles_movs - min_movs);
    ordenada = (MAX_PUNTOS - MIN_PUNTOS) * min_movs / (posibles_movs - min_movs) + MAX_PUNTOS;

    //Redondeamos hacia abajo(sumando 0.5 nos aseguramos de que el redondeo sea correcto)
    puntos_partida = floor(puntos_partida + actualMov() * pendiente + ordenada + 0.5);

    //CONTAMOS PUNTOS POR LONGITUD DE LINEA, siendo, segun esta funcion lineal, 50 puntos si consigues ganar con una linea de 19 fichas o 10 si ganas con una linea de 3.
    puntos_partida = puntos_partida + fichasLinea()*2.5 + 2.5;
}

Tablero Tablero::nextMove(char &last_acc) const {
    last_acc++;
   
    while (last_acc <= 'g' and !possibleAct())
        last_acc++;
    
    if(last_acc>'g')
        return *this;
    else{
        Tablero tab_new(*this);
        tab_new.insertar2(last_acc);
        tab_new.cambiarTurno();
        return tab_new;
    }
}
//
//vector<Tablero> Tablero::generateMoves() const {
//    vector<Tablero> movs;
//
//    for (int i = 0; i < 7; i++) {
//        Tablero taux(*this);
//
//        if (tablero.getElemento(0, i) == 0) {
//            taux.insertar('a' + i);
//            movs.push_back(taux);
//        }
//    }
//
//    return movs;
//}

int Tablero::possibleAct() const {
    int poss = 0;

    for (int i = 0; i < 7; i++)
        if (tablero.getElemento(0, i) == 0)
            poss++;

    return poss;
}



