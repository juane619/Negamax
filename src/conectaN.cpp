#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "tablero.h"
#include "jugador.h"

using namespace std;

//Funcion para guardar la partida

void Guardar(const Jugador &pl1, const Jugador &pl2, const Tablero &t, const char* file_save) {
    ofstream conf(file_save);
    conf << "#MP-CONECTAN-1.0" << '\n';
    conf << pl1;
    conf << pl2;
    conf << t;
    conf.close();
}

//Funcion para cargar la partida

bool Cargar(Jugador &pl1, Jugador &pl2, Tablero &t, const char* file_load) {
    const char FILE_VALIDO[] = "MP-CONECTAN-1.0";
    char type_file[16] = "";
    bool es_valido = true;
    ifstream conf(file_load);

    char c;
    if (conf) {
        do {
            conf >> c;

            if (c == '#')
                conf.getline(type_file, 1024); // Suponemos una línea tiene menos de 1024
            else
                conf >> c;
        } while (c != '#');

        //COMPROBAMOS LA PRIMERA LINEA DEL ARCHIVO DE CARGA CON LA FRASE VALIDA PARA ARCHIVOS DE CARGA PARA PODER CONTINUAR
        if (strcmp(type_file, FILE_VALIDO) != 0)
            es_valido = false;
    } else
        return 0;

    if (es_valido) {
        conf >> pl1 >> pl2 >> t;
        return 1;
    } else
        return 0;
}

//Funcion que muestra el diálogo para guardar la partida

bool DialogoSalvar() {
    char respuesta;

    do {
        cout << "¿Quiere guardar la partida? (S/N)";
        cin >> respuesta;
        respuesta = toupper(respuesta);
    } while (respuesta != 'S' && respuesta != 'N');

    return respuesta == 'S';
}

int main(int argc, char** argv) {
    char respuesta;
    int tur_actual;
    bool escogido_bien = false;
    bool play_ready = false;
    char file_save[30];
    Jugador pl1, pl2;
    Tablero tab;

    system("clear");
    cout << "----------------------------------------" << endl;
    cout << "-----------CUATRO EN RAYA---------------" << endl;
    cout << "----------------------------------------" << endl << endl;

    if (argc == 1) {
        char nombre[200];
        int filas, columnas, fichas, fichas_t;
        int fichas_align_max;

        do {
            if (!cin.good()) {
                // Resetear las banderas de error:
                cin.clear();
                // Vaciar el contenido:
                cin.ignore(1000, '\n');
            }

            cout << "Introduzca filas (4-20): ";
            cin >> filas;
        } while (filas < 4 || filas > 20);

        do {
            if (!cin.good()) {
                cin.clear();
                cin.ignore(1000, '\n');
            }

            cout << "Introduzca columnas (4-20): ";
            cin >> columnas;
        } while (columnas < 4 || columnas > 20);

        fichas_align_max = min(filas, columnas) - 1;
        do {
            if (!cin.good()) {
                cin.clear();
                cin.ignore(1000, '\n');
            }

            cout << "Introduzca fichas a alinear (3-" << fichas_align_max << "): ";
            cin >> fichas;
        } while (fichas < 3 || fichas > fichas_align_max);

        do {
            if (!cin.good()) {
                cin.clear();
                cin.ignore(1000, '\n');
            }

            cout << "Introduzca fichas a insertar por turno (1-" << fichas - 2 << "): ";
            cin >> fichas_t;
        } while (fichas_t < 1 || fichas_t > fichas - 2);

        tab = Tablero(filas, columnas, fichas, fichas_t);

        cout << "Introduzca el nombre del primer jugador: ";
        cin >> nombre;
        pl1.setNombre(nombre);

        cout << "Introduzca el nombre del segundo jugador: ";
        cin >> nombre;
        pl2.setNombre(nombre);

        play_ready = true;
    } else if (argc == 2) {
        if (Cargar(pl1, pl2, tab, argv[1]))
            play_ready = true;
    }

    if (play_ready) {
        do {
            tab.prettyPrint();
            tur_actual = tab.turnoActual();

            //CONTROL DE TURNOS
            if (tur_actual == 1)
                escogido_bien = pl1.escogeColumna(tab);
            else if (tur_actual == 2)
                escogido_bien = pl2.escogeColumna(tab);

            if (!escogido_bien) {
                cerr << "Error en la seleccion de columna.";

                if (DialogoSalvar()) {
                    cout << "Introduzca nombre de archivo: ";
                    cin >> file_save;

                    Guardar(pl1, pl2, tab, file_save);
                }
            }else
                tab.cambiarTurno();

            if (tab.gameEnd()) {
                tab.prettyPrint();

                if (tab.hayGanador()) {
                    if (tab.getGanador() == 1)
                        pl1.actualizaPuntos(tab);
                    else
                        pl2.actualizaPuntos(tab);
                } else
                    pl1.empata();

                cout << endl << "Resultados tras esta partida: " << endl <<
                        pl1.getNombre() << ": " << pl1.getGanadas() << " ganadas que acumulan " << pl1.getPuntos() << " puntos." << endl <<
                        pl2.getNombre() << ": " << pl2.getGanadas() << " ganadas que acumulan " << pl2.getPuntos() << " puntos." << endl;

                do {
                    cout << endl << "¿Jugar de nuevo? (S/N)";
                    cin >> respuesta;
                    respuesta = toupper(respuesta);
                } while (respuesta != 'S' && respuesta != 'N');

                if (respuesta == 'S')
                    tab.vaciarTablero();

                else if (respuesta == 'N') {
                    cout << "RESULTADOS FINALES: " << endl <<
                            pl1.getNombre() << ": " << pl1.getGanadas() << " ganadas que acumulan " << pl1.getPuntos() << " puntos." << endl <<
                            pl2.getNombre() << ": " << pl2.getGanadas() << " ganadas que acumulan " << pl2.getPuntos() << " puntos." << endl <<
                            pl1.getEmpatadas() << " empatadas." << endl;

                    if (DialogoSalvar()) {
                        tab.vaciarTablero();
                        cout << "Introduzca nombre de archivo: ";
                        cin >> file_save;
                        Guardar(pl1, pl2, tab, file_save);
                        tab.setTurno(0);
                    }
                }
            }
        } while (!tab.gameEnd());
    } else
        cerr << "Problema al cargar el archivo de configuracion. Saliendo.." << endl;
}
