#ifndef HAMILTON_HPP
#define HAMILTON_HPP

#include <vector>

std::vector<std::vector<int>> filtrarPorLinkage(const std::vector<std::vector<int>>&pesos,int l);

std::vector<int> buscarCaminoHamiltoniano(const std::vector<std::vector<int>>&pesos);

#endif

