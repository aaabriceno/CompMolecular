#ifndef MATRIZ_HPP
#define MATRIZ_HPP

#include <string>
#include <vector>
#include <functional>

std::vector<std::vector<double>> construirMatrizDistancias(const std::vector<std::string>& secuencias);

std::vector<std::vector<double>> construirMatrizDistancias(
    const std::vector<std::string>& secuencias,
    std::function<double(const std::string&, const std::string&)> funcionDistancia);

#endif
