#ifndef MAQUINA_H
#define MAQUINA_H

#include <string>

namespace planta {

// Clase base abstracta de todas las maquinas de la planta.
//
// CONTRATO para toda clase derivada:
// 1. ejecutarCiclo() realiza el trabajo fisico de un ciclo.
// 2. tiempoCicloSeg() devuelve los segundos de un ciclo;
//    siempre debe ser mayor que cero.
//
// La base registra las piezas y el tiempo de cada ciclo.

class Maquina {
private:
    std::string nombre;
    int piezasProcesadas;
    int tiempoTrabajadoSeg;

    void registrarPieza();
    void agregarTiempo(int segundos);
    int tiempoCicloValidado() const;

    virtual void ejecutarCiclo() = 0;

public:
    explicit Maquina(const std::string& nombre);
    virtual ~Maquina() = default;

    void procesar();

    virtual int tiempoCicloSeg() const = 0;

    double capacidadPorHora() const;

    const std::string& getNombre() const;
    int getPiezasProcesadas() const;
    int getTiempoTrabajadoSeg() const;

    void mostrarEstado() const;
};

} // namespace planta

#endif