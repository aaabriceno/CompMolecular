#include <iostream>
#include "consenso/consenso.hpp"
#include "hamilton/hamilton.hpp"
#include "grafo/grafo.hpp"

using namespace std;

int main(){
    vector<string> fragmentosOriginales = {
        "ATCCGTTGAAGCCGCGGGC",
        "TTAACTCGAGG",
        "TTAAGTACTGCCCG",
        "ATCTGTGTCGGG",
        "CGACTCCCGACACA",
        "CACAGATCCGTTGAAGCCGCGGG",
        "CTCGAGTTAAGTA",
        "CGCGGGCAGTACTT"
    };
    int l = 55; // tamaño objetivo aproximado de la molécula destino

    // Se recalcula 2.1 solo para obtener el grafo de fragmentos y el
    // linkage t que sirve de parametro de entrada para 2.2.
    auto candidatos = generarCandidatosPorOrientacion(fragmentosOriginales);
    auto resultado = escogerPorTamanoObjetivo(candidatos, l);
    int t = resultado.linkageMasDebil;

    // ---- 2.2 Subgrafos Aciclicos ----
    auto pesosFiltrados = filtrarPorLinkage(resultado.pesos, t);
    auto caminoFiltrado = buscarCaminoHamiltoniano(pesosFiltrados);

    cout << "=== 2.2 Subgrafos Aciclicos (t = " << t << ") ===\n";
    if (caminoFiltrado.empty()) {
        cout << "No existe camino Hamiltoniano en el subgrafo filtrado.\n";
        return 0;
    }

    string superstringFiltrado = resultado.fragmentos[caminoFiltrado[0]].secuencia;
    for (size_t k = 0; k + 1 < caminoFiltrado.size(); ++k) {
        int f = caminoFiltrado[k], g = caminoFiltrado[k + 1];
        superstringFiltrado += resultado.fragmentos[g].secuencia.substr(pesosFiltrados[f][g]);
    }

    cout << "Camino: ";
    for (int idx : caminoFiltrado) cout << resultado.fragmentos[idx].id << " ";
    cout << "\nSuperstring: " << superstringFiltrado << "\n";
    cout << "Longitud: " << superstringFiltrado.size() << "\n";

    generarDotOverlap(resultado.fragmentos, pesosFiltrados, "hamilton_grafo.dot");
    cout << "Subgrafo filtrado generado: hamilton_grafo.dot\n";
    cout << "Renderizar con: dot -Tpng hamilton_grafo.dot -o hamilton_grafo.png\n\n";

    generarDotCamino(resultado.fragmentos, pesosFiltrados, caminoFiltrado, "hamilton.dot");
    cout << "Grafo del camino generado: hamilton.dot\n";
    cout << "Renderizar con: dot -Tpng hamilton.dot -o hamilton.png\n";

    return 0;
}