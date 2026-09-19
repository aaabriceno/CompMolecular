#ifndef CONSENSO_HPP
#define CONSENSO_HPP

#include <string>
#include <vector>

struct Fragmento {
    std::string id;
    std::string secuencia;
    bool esReverso; // true si se usó el complemento-reverso del fragmento original
};

struct Arista {
    int origen;
    int destino;
    int peso; // t: cantidad de bases solapadas (suffix(origen,t) == prefix(destino,t))
};

struct ResultadoConsenso {
    std::string superstring;
    std::vector<int> caminoHamiltoniano; // orden de índices en 'fragmentos' que da el superstring
    std::vector<Arista> aristasSeleccionadas;
    int linkageMasDebil; // menor peso de arista usado en el camino final
    std::vector<Fragmento> fragmentos; // fragmentos (con orientación fija) usados para este resultado
    std::vector<std::vector<int>> pesos; // multigrafo de overlaps usado para este resultado
};

// Genera el complemento-reverso de una secuencia de ADN.
std::string complementoReverso(const std::string& secuencia);

// A partir de los fragmentos originales, genera el conjunto ampliado
// considerando tanto la secuencia directa como su complemento-reverso.
std::vector<Fragmento> generarFragmentosConReversos(const std::vector<std::string>& fragmentosOriginales);

// Calcula el peso del overlap entre 'a' y 'b': el mayor t tal que
// suffix(a, t) == prefix(b, t). Devuelve 0 si no hay overlap.
int calcularOverlap(const std::string& a, const std::string& b);

// Construye el multigrafo de overlaps OM(F) como matriz de pesos
// entre todos los pares de fragmentos (incluye directos y reversos).
std::vector<std::vector<int>> construirMultigrafoOverlap(const std::vector<Fragmento>& fragmentos);

// Algoritmo greedy (Union-Find) que busca el camino Hamiltoniano de
// máxima ponderación en el multigrafo de overlaps, equivalente a
// encontrar el superstring común más corto (Eq. 8, slide 06b pp.28).
ResultadoConsenso ensamblarPorGreedy(const std::vector<Fragmento>& fragmentos,
                                     const std::vector<std::vector<int>>& pesos);

// Entre las variantes de complemento-reverso posibles para un mismo
// conjunto, escoge la solución cuyo superstring se aproxime más al
// tamaño objetivo 'l'.
ResultadoConsenso escogerPorTamanoObjetivo(const std::vector<ResultadoConsenso>& candidatos, int l);

// Prueba las 2^n combinaciones de orientación (directa o complemento-reverso)
// para los n fragmentos originales. Por cada combinación construye su propio
// multigrafo de overlaps (n nodos, una orientación fija por fragmento) y
// corre el ensamblaje greedy. Devuelve un candidato por combinación.
std::vector<ResultadoConsenso> generarCandidatosPorOrientacion(const std::vector<std::string>& fragmentosOriginales);

#endif
