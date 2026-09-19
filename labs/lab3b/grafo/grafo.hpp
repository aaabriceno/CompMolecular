#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <string>
#include <vector>
#include "../consenso/consenso.hpp"

// Genera un archivo .dot (Graphviz) con el multigrafo de overlaps: un nodo
// por fragmento y una arista dirigida f->g con el peso t si pesos[f][g] > 0.
// Estilo equivalente al ejemplo de "Subgrafos Acíclicos" (slide 06b pp.39).
void generarDotOverlap(const std::vector<Fragmento>& fragmentos,
                        const std::vector<std::vector<int>>& pesos,
                        const std::string& rutaArchivo);

// Genera un archivo .dot que resalta únicamente el camino Hamiltoniano
// encontrado (las aristas usadas en el ensamblaje final), útil para
// visualizar el resultado de 2.1 o 2.2 por separado del grafo completo.
void generarDotCamino(const std::vector<Fragmento>& fragmentos,
                       const std::vector<std::vector<int>>& pesos,
                       const std::vector<int>& camino,
                       const std::string& rutaArchivo);

#endif
