#ifndef NW_HPP
#define NW_HPP

#include <string>
#include <vector>

const int MATCH = 1;
const int MISMATCH = -1;
const int GAP = -2;

std::vector<std::vector<int>> construirMatriz(const std::string& s, const std::string& t);

void traceback(int i, int j, std::string alignS, std::string alignT,
                std::vector<std::vector<int>>& M, const std::string& s, const std::string& t,
                std::vector<std::pair<std::string,std::string>>& soluciones);

std::pair<std::string,std::string> tracebackUnico(std::vector<std::vector<int>>& M,
                                                    const std::string& s, const std::string& t);

#endif
