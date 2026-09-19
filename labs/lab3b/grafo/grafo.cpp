#include "grafo.hpp"
#include <fstream>
#include <set>

void generarDotOverlap(const std::vector<Fragmento>& fragmentos,
                        const std::vector<std::vector<int>>& pesos,
                        const std::string& rutaArchivo) {
    std::ofstream out(rutaArchivo);
    out << "digraph OverlapGraph {\n";
    out << "  rankdir=LR;\n";

    size_t n = fragmentos.size();
    for (size_t i = 0; i < n; ++i) {
        out << "  \"" << fragmentos[i].id << "\" [shape=circle];\n";
    }
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (pesos[i][j] > 0) {
                out << "  \"" << fragmentos[i].id << "\" -> \"" << fragmentos[j].id
                    << "\" [label=\"" << pesos[i][j] << "\"];\n";
            }
        }
    }
    out << "}\n";
}

void generarDotCamino(const std::vector<Fragmento>& fragmentos,
                       const std::vector<std::vector<int>>& pesos,
                       const std::vector<int>& camino,
                       const std::string& rutaArchivo) {
    std::ofstream out(rutaArchivo);
    out << "digraph CaminoHamiltoniano {\n";
    out << "  rankdir=LR;\n";

    for (int idx : camino) {
        out << "  \"" << fragmentos[idx].id << "\" [shape=circle];\n";
    }
    for (size_t k = 0; k + 1 < camino.size(); ++k) {
        int f = camino[k], g = camino[k + 1];
        out << "  \"" << fragmentos[f].id << "\" -> \"" << fragmentos[g].id
            << "\" [label=\"" << pesos[f][g] << "\"];\n";
    }
    out << "}\n";
}
