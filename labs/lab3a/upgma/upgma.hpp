#ifndef UPGMA_HPP
#define UPGMA_HPP

#include <string>
#include <vector>
#include <memory>

struct NodoUPGMA {
    std::string nombre;
    double altura;
    std::shared_ptr<NodoUPGMA> izquierdo;
    std::shared_ptr<NodoUPGMA> derecho;

    NodoUPGMA(const std::string& n)
        : nombre(n), altura(0.0), izquierdo(nullptr), derecho(nullptr) {}
};

std::shared_ptr<NodoUPGMA> upgma(std::vector<std::vector<double>> matriz,
                                  std::vector<std::string> nombres);

#endif
