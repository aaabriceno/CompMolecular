#ifndef NW_H
#define NW_H

#include <string>
#include <vector>

const int MATCH = 1;
const int MISMATCH = -1;
const int GAP = -2;

std::vector<std::vector<int>> construirMatriz(const std::string& s, const std::string& t);

void traceback(int i, int j, std::string alignS, std::string alignT,
                std::vector<std::vector<int>>& M, const std::string& s, const std::string& t,
                std::vector<std::pair<std::string,std::string>>& soluciones);

// Traceback O(n+m): una sola solucion optima, eligiendo en cada empate
// el camino que produce menos rupturas (bloques de gaps) en el alineamiento.
std::pair<std::string,std::string> tracebackUnico(std::vector<std::vector<int>>& M,
                                                    const std::string& s, const std::string& t);

#endif
