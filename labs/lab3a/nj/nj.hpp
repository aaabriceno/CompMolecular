#ifndef NJ_HPP
#define NJ_HPP

#include <string>
#include <vector>
#include <memory>

struct NodoNJ {
    std::string nombre;
    std::shared_ptr<NodoNJ> izquierdo;
    std::shared_ptr<NodoNJ> derecho;
    double longIzquierdo;
    double longDerecho;

    NodoNJ(const std::string& n)
        : nombre(n), izquierdo(nullptr), derecho(nullptr),
          longIzquierdo(0.0), longDerecho(0.0) {}
};

std::shared_ptr<NodoNJ> neighborJoining(std::vector<std::vector<double>> matriz,
                                         std::vector<std::string> nombres);

#endif
