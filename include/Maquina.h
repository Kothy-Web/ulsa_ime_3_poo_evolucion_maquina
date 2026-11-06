#ifndef MAQUINA_H
#define MAQUINA_H

#include <string>

class Maquina {
private:
    std::string nombre;
    int piezasProcesadas;
    int tiempoTrabajadoSeg;

protected:
    void registrarPieza();
    void agregarTiempo(int segundos);

public:
    explicit Maquina(const std::string& nombre);

    virtual ~Maquina() = default;
// CONTRATO de procesar(): cada llamada procesa exactamente UNA pieza.
    // Toda clase derivada debe llamar una vez a registrarPieza() y una vez
    // a agregarTiempo(tiempoCicloSeg()).

    virtual void procesar() = 0;
    virtual int tiempoCicloSeg() const = 0;
 // CONTRATO de tiempoCicloSeg(): devuelve los segundos que tarda un
    // ciclo. Siempre es mayor que cero.
    double capacidadPorHora() const;

    const std::string& getNombre() const;
    int getPiezasProcesadas() const;
    int getTiempoTrabajadoSeg() const;

    void mostrarEstado() const;
};

#endif