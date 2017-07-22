#ifndef JUGADOR_H
#define JUGADOR_H

#include <iostream>
#include "tablero.h"
using namespace std;

class Tablero;

class Jugador {
private:
    const double INF_TOP= 9999999999.0;
    const double INF_DOWN= -9999999999.0;
    
    int jugador;
    char* nombre; //!< Member variable
    int ganadas; //!< Member variable "ganadas"
    int empatadas; //!< Member variable "empatadas"
    int puntos_acumulados; //!< Member variable "puntos_acumulados"
    bool automatico;
    char dificultad;
    
    /*IMPLEMENTACION NEGAMAX*/
    double Negamax(const Tablero &tabtmp, int jugador, int profundidad, double alfa, double beta, char &columna_elegida);
    double evaluaJugada(const Tablero &tab, int jugador);
    int nLineas(const Tablero &tab, int tam_line, int jugador);
    
    /////////////////////////////////
public:
    /** Default constructor */
    Jugador();

    ~Jugador() {
        delete[] nombre;
    }

    Jugador(const Jugador& j);

    Jugador& operator=(const Jugador& j);

    friend istream& operator>>(istream &flujo, Jugador &j);

    friend ostream& operator<<(ostream &flujo, const Jugador &j);

    /** Access nombre[50]
     * \return The current value of nombre[50]
     */
    const char* getNombre() const {
        return nombre;
    }

    /** Access ganadas
     * \return The current value of ganadas
     */
    int getGanadas() const {
        return ganadas;
    }

    /** Access empatadas
     * \return The current value of empatadas
     */
    int getEmpatadas() const {
        return empatadas;
    }

    /** Access puntos_acumulados
     * \return The current value of puntos_acumulados
     */
    int getPuntos() const {
        return puntos_acumulados;
    }

    char getDificultad() const {
        return dificultad;
    }

    /** Set nombre[50]
     * \param val New value to set
     */
    void setNombre(const char* nom);

    /** Set empatadas
     * \param val New value to set
     */
    void empata() {
        empatadas = empatadas + 1;
    }

    /** Set puntos_acumulados
     * \param val New value to set
     */
    void setPuntos(int val) {
        puntos_acumulados = val;
    }

    /** Escoger columna
     * \param val New value to set
     */
    bool escogeColumna(Tablero& tabl);

    void actualizaPuntos(const Tablero& t) {
        puntos_acumulados = puntos_acumulados + t.getPuntos();
        ganadas++;
    }
    
    int ComprobarAdyacentes(const Tablero &estado, int jugador, int nivel);
};

#endif // JUGADOR_H
