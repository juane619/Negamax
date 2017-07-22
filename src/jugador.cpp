#include <cstring>
#include <iostream>
#include <cstdlib>
#include "jugador.h"

using namespace std;

Jugador::Jugador() {
    nombre = NULL;
    ganadas = empatadas = puntos_acumulados = 0;
    automatico = false;
    jugador = 0;
}

Jugador::Jugador(const Jugador& j) {
    ganadas = j.ganadas;
    empatadas = j.empatadas;
    puntos_acumulados = j.puntos_acumulados;
    automatico = j.automatico;
    dificultad = j.dificultad;

    int long_n = strlen(j.nombre);
    nombre = new char[long_n];

    for (int i = 0; i < long_n; i++)
        nombre[i] = j.nombre[i];

    nombre[long_n] = '\0';

    jugador = j.jugador;
}

Jugador& Jugador::operator=(const Jugador& j) {
    if (&j != this) {
        ganadas = j.ganadas;
        empatadas = j.empatadas;
        puntos_acumulados = j.puntos_acumulados;
        automatico = j.automatico;
        dificultad = j.dificultad;

        delete[] nombre;
        int long_n = strlen(j.nombre);
        nombre = new char[long_n];

        for (int i = 0; i < long_n; i++)
            nombre[i] = j.nombre[i];

        nombre[long_n] = '\0';
        jugador = j.jugador;
    }

    return *this;
}

istream& operator>>(istream& is, Jugador& j) {
    char c;

    do {
        is >> c;

        if (c != '#')
            is.ignore(1024, '\n'); // Suponemos una línea tiene menos de 1024
    } while (c != '#');

    char nombre[200];
    is.getline(nombre, 1024);
    j.setNombre(nombre);
    is >> j.ganadas >> j.empatadas >> j.puntos_acumulados >> j.automatico >> j.dificultad;

    return is;
}

ostream& operator<<(ostream &flujo, const Jugador& j) {
    flujo << '#' << j.nombre << '\n' << j.ganadas << ' ' << j.empatadas << ' ' << j.puntos_acumulados << ' ' << j.automatico << j.dificultad << '\n';

    return flujo;
}

void Jugador::setNombre(const char* nom) {
    int longitud_n = strlen(nom) + 1;

    nombre = new char[longitud_n];

    for (int i = 0; i < longitud_n; i++)
        nombre[i] = nom[i];

    nombre[longitud_n] = '\0';

    if (nombre[0] == '@') {
        jugador = 2;
        char dificul;
        automatico = true;

        do {
            if (!cin.good()) {
                cin.clear();
                cin.ignore(1000, '\n');
            }

            cout << "Eliga la dificultad del juego (baja: b o media: m): ";
            cin >> dificul;
            dificul = toupper(dificul);
        } while (dificul != 'B' && dificul != 'M');

        dificultad = dificul;
    }
}

bool Jugador::escogeColumna(Tablero& tabl) {
    char col_esco = 'a' - 1;
    char tope_col = 'a' + tabl.columnas() - 1;

    if (!automatico) {
        cout << getNombre() << ", escoja una columna (letra a-" << tope_col << ") para la ficha " << tabl.fichaTurnoActual() << " de " << tabl.fichasTurno() << ": ";
        cin >> col_esco;

        if (col_esco < 'a' || col_esco > tope_col)
            return 0;

    } else {
        //ESCOGIMIENTO DE COLUMNA PARA EL JUGADOR AUTOMATICO CON NEGAMAX
        int profundidad;
        
        if (getDificultad() == 'B')
            profundidad = 4;
        else if (getDificultad() == 'M')
            profundidad = 6;

        Negamax(tabl, jugador, profundidad, INF_DOWN, INF_TOP, col_esco);

        if (col_esco < 'a' || col_esco > tope_col)
            return 0;
    }
    tabl.insertar2(col_esco);

    return 1;
}

/////////////////////////*COMPROBAR ADYACENCIAS*///////////////////////////

int AdyacenteVertical(const Tablero &estado, int row, int col, int jugador, int nivel) {

    int consecutivas = 0;

    for (int i = row; i < 7; i++) {
        if (estado.getPosicion(i, col) == jugador)
            consecutivas += 1;
        else
            break;
    }

    if (consecutivas >= nivel)
        return 1;
    else
        return 0;
}

int AdyacenteHorizontal(const Tablero &estado, int row, int col, int jugador, int nivel) {

    int consecutivas = 0;

    for (int j = col; j < 7; j++) {
        if (estado.getPosicion(row, j) == jugador)
            consecutivas += 1;
        else
            break;
    }

    if (consecutivas >= nivel)
        return 1;
    else
        return 0;

}

int AdyacenteDiagonal(const Tablero &estado, int row, int col, int jugador, int nivel) {

    int total = 0;

    // Comprobamos los diagonales ascendentes
    int consecutivas = 0;

    int j = col;

    for (int i = row; i < 7; i++) {
        if (j > 6) // Control para no pasarnos del maximo de columnas
            break;
        else if (estado.getPosicion(i, j) == jugador)
            consecutivas += 1;
        else
            break;
        j += 1; // Incrementamos la columna cuando se incrementa la fila

    }

    if (consecutivas >= nivel)
        total += 1;

    // Comprobamos los diagonales descendentes
    consecutivas = 0;
    j = col;
    for (int i = row; i >= 0; i--) {
        if (j > 6) // Control para no pasarnos del maximo de columnas
            break;
        else if (estado.getPosicion(i, j) == jugador)
            consecutivas += 1;
        else
            break;
        j += 1; // Incrementamos la columna cuando se decrementa la fila

    }

    if (consecutivas >= nivel)
        total += 1;

    return total;

}

int Jugador::ComprobarAdyacentes(const Tablero &estado, int jugador, int nivel) {

    int count = 0;

    // Para cada ficha del tablero...
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {

            // ...si es del color del jugador...
            if (estado.getPosicion(i, j) == jugador) {
                // Comprueba los verticales de la columna
                count += AdyacenteVertical(estado, i, j, jugador, nivel);

                // Comprueba los horizontales de la fila
                count += AdyacenteHorizontal(estado, i, j, jugador, nivel);

                // Comprueba los diagonales (los dos tipos)
                count += AdyacenteDiagonal(estado, i, j, jugador, nivel);
            }
        }
    }

    // Devolvemos la suma de los adyacentes encontrados para el "nivel"
    return count;

}

///////////////////////////////////////////////////////////////////////////


//FUNCIONES NECESARIAS PARA LA IMPLEMENTACION DEL ALGORITMO NEGAMAX(APROXIMACIÓN)

//Algortitmo recursivo Negamax
double Jugador::Negamax(const Tablero &tab, int jugador_p, int profundidad, double alfa, double beta, char &columna_elegida) {
    char accion_anterior;
    double aux;

    int n_act= tab.possibleAct();
    
    if(profundidad==0 || n_act==0){
        return evaluaJugada(tab, jugador_p);
    }else{
        char ult_act='a'-1;
        Tablero hijo= tab.nextMove(ult_act);
        double mejor= INF_DOWN;
        
        while(!(hijo==tab)){
            aux= -Negamax(hijo, jugador_p, profundidad-1, -beta, -alfa, accion_anterior);
            
            if(aux > mejor){
                mejor = aux;
                columna_elegida= ult_act;
            }
            
            if(alfa>=beta)
                break;
            
            hijo= tab.nextMove(ult_act);
        }
        
        return mejor;
    }
}

//Funcion heurística (de evaluación)
double Jugador::evaluaJugada(const Tablero &tab, int jugador) {
    int total, n1_2, n1_3, n1_4, n2_2, n2_3, n2_4;

    int jug_op = 1;

    if (jugador == 1)
        jug_op = 2;

    n1_2 = ComprobarAdyacentes(tab, jugador, 2);
    n1_3 = ComprobarAdyacentes(tab, jugador, 3);
    n1_4 = ComprobarAdyacentes(tab, jugador, 4);

    n2_2 = ComprobarAdyacentes(tab, jug_op, 2);
    n2_3 = ComprobarAdyacentes(tab, jug_op, 3);
    n2_4 = ComprobarAdyacentes(tab, jug_op, 4);

    total = (n1_2 * 10 + n1_3 * 100 + n1_4 * 1000000) - (n2_4 * 1000000 + n2_3 * 100 + n2_2 * 10);
    
    return total;
}
