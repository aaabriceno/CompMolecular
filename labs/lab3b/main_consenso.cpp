#include <iostream>
#include "consenso/consenso.hpp"
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

    // ---- 2.1 Secuencia de Consenso ----
    auto candidatos = generarCandidatosPorOrientacion(fragmentosOriginales);
    auto resultado = escogerPorTamanoObjetivo(candidatos, l);

    cout << "=== 2.1 Secuencia de Consenso ===\n";
    cout << "Superstring: " << resultado.superstring << "\n";
    cout << "Longitud: " << resultado.superstring.size() << "\n";
    cout << "Linkage mas debil (t): " << resultado.linkageMasDebil << "\n\n";

    generarDotOverlap(resultado.fragmentos, resultado.pesos, "consenso_grafo.dot");
    cout << "Grafo completo generado: consenso_grafo.dot\n";
    cout << "Renderizar con: dot -Tpng consenso_grafo.dot -o consenso_grafo.png\n\n";

    generarDotCamino(resultado.fragmentos, resultado.pesos, resultado.caminoHamiltoniano, "consenso.dot");
    cout << "Grafo del camino generado: consenso.dot\n";
    cout << "Renderizar con: dot -Tpng consenso.dot -o consenso.png\n";

    return 0;
}