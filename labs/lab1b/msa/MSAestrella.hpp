#ifndef MSAESTRELLA_HPP
#define MSAESTRELLA_HPP

#include <vector>
#include <string>

using MSA = std::vector<std::string>; //esto es las secuencias ya alineadas, misma longitud con un "-"

// Matriz NxN de scores optimos pairwise (NW) entre todas las secuencias
std::vector<std::vector<int>> matrizScoresPairwise(const std::vector<std::string>& secuencias);

// Indice de la secuencia "centro": la que maximiza la suma de sus scores contra las demas
int elegirCentro(const std::vector<std::vector<int>>& scores);

struct ResultadoMSA{
    MSA alineamiento;
    int scoreSP;
};

std::vector<ResultadoMSA> msaEstrella(const std::vector<std::string>& seqs);

// version que arma TODAS las combinaciones posibles de MSA, considerando
// todos los empates de traceback en cada alineamiento pairwise contra el centro.
// uso esta version solo quiero verificar que existan varias soluciones optimas
// (para el ejemplo de clase), porque el numero de combinaciones puede crecer
// muy rapido si hay muchos empates o muchas secuencias.
std::vector<ResultadoMSA> msaEstrellaTodasLasSoluciones(const std::vector<std::string>& seqs);

int scoreSumaDePares(const MSA& msa);

#endif