#include "consenso.hpp"
#include <algorithm>
#include <numeric>

std::string complementoReverso(const std::string& secuencia) {
    std::string resultado(secuencia.rbegin(), secuencia.rend());
    for (char& base : resultado) {
        switch (base) {
            case 'A': base = 'T'; break;
            case 'T': base = 'A'; break;
            case 'C': base = 'G'; break;
            case 'G': base = 'C'; break;
            default: break;
        }
    }
    return resultado;
}

std::vector<Fragmento> generarFragmentosConReversos(const std::vector<std::string>& fragmentosOriginales) {
    std::vector<Fragmento> resultado;
    resultado.reserve(fragmentosOriginales.size() * 2);
    for (size_t i = 0; i < fragmentosOriginales.size(); ++i) {
        std::string id = "f" + std::to_string(i + 1);
        resultado.push_back({id, fragmentosOriginales[i], false});
        resultado.push_back({id + "'", complementoReverso(fragmentosOriginales[i]), true});
    }
    return resultado;
}

int calcularOverlap(const std::string& a, const std::string& b) {
    int maxT = static_cast<int>(std::min(a.size(), b.size()));
    for (int t = maxT; t > 0; --t) {
        if (a.compare(a.size() - t, t, b, 0, t) == 0) {
            return t;
        }
    }
    return 0;
}



std::vector<std::vector<int>> construirMultigrafoOverlap(const std::vector<Fragmento>& fragmentos) {
    size_t n = fragmentos.size();
    std::vector<std::vector<int>> pesos(n, std::vector<int>(n, 0));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (i == j) continue;
            pesos[i][j] = calcularOverlap(fragmentos[i].secuencia, fragmentos[j].secuencia);
        }
    }
    return pesos;
}

namespace {

struct UnionFind {
    std::vector<int> padre;
    explicit UnionFind(int n) : padre(n) {
        std::iota(padre.begin(), padre.end(), 0);
    }
    int encontrar(int x) {
        if (padre[x] != x) padre[x] = encontrar(padre[x]);
        return padre[x];
    }
    void unir(int x, int y) {
        padre[encontrar(x)] = encontrar(y);
    }
};

} // namespace

ResultadoConsenso ensamblarPorGreedy(const std::vector<Fragmento>& fragmentos,
                                      const std::vector<std::vector<int>>& pesos) {
    int n = static_cast<int>(fragmentos.size());

    std::vector<Arista> todasLasAristas;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j && pesos[i][j] > 0) {
                todasLasAristas.push_back({i, j, pesos[i][j]});
            }
        }
    }
    std::sort(todasLasAristas.begin(), todasLasAristas.end(),
              [](const Arista& a, const Arista& b) { return a.peso > b.peso; });

    std::vector<int> inGrado(n, 0), outGrado(n, 0);
    UnionFind uf(n);
    std::vector<Arista> seleccionadas;
    std::vector<int> siguiente(n, -1);
    int componentes = n;

    for (const auto& arista : todasLasAristas) {
        int f = arista.origen, g = arista.destino;
        if (outGrado[f] == 0 && inGrado[g] == 0 && uf.encontrar(f) != uf.encontrar(g)) {
            seleccionadas.push_back(arista);
            siguiente[f] = g;
            inGrado[g] = 1;
            outGrado[f] = 1;
            uf.unir(f, g);
            componentes--;
        }
        if (componentes == 1) break;
    }

    // El greedy puede dejar más de un componente (nodos con solo overlaps
    // débiles quedan aislados si sus aristas chocan con nodos ya ocupados).
    // Para garantizar que el superstring final incluya TODOS los fragmentos,
    // se enlazan los componentes restantes entre sí (sin overlap, peso 0).
    while (componentes > 1) {
        int colaLibre = -1, cabezaLibre = -1;
        for (int i = 0; i < n; ++i) {
            if (outGrado[i] == 0) { colaLibre = i; break; }
        }
        for (int j = 0; j < n; ++j) {
            if (inGrado[j] == 0 && uf.encontrar(j) != uf.encontrar(colaLibre)) { cabezaLibre = j; break; }
        }
        siguiente[colaLibre] = cabezaLibre;
        inGrado[cabezaLibre] = 1;
        outGrado[colaLibre] = 1;
        uf.unir(colaLibre, cabezaLibre);
        componentes--;
    }

    // Reconstruir el camino Hamiltoniano a partir del nodo inicial (in-grado 0).
    int inicio = 0;
    for (int i = 0; i < n; ++i) {
        if (inGrado[i] == 0) { inicio = i; break; }
    }

    std::vector<int> camino;
    int actual = inicio;
    camino.push_back(actual);
    while (siguiente[actual] != -1) {
        actual = siguiente[actual];
        camino.push_back(actual);
    }

    // Construir el superstring concatenando el primer fragmento completo
    // y, por cada arista del camino, solo el sufijo no solapado del siguiente.
    std::string superstring = fragmentos[camino[0]].secuencia;
    int linkageMasDebil = -1;
    for (size_t k = 0; k + 1 < camino.size(); ++k) {
        int f = camino[k], g = camino[k + 1];
        int t = pesos[f][g];
        superstring += fragmentos[g].secuencia.substr(t);
        if (linkageMasDebil == -1 || t < linkageMasDebil) linkageMasDebil = t;
    }
    if (linkageMasDebil == -1) linkageMasDebil = 0;

    return {superstring, camino, seleccionadas, linkageMasDebil, fragmentos, pesos};
}

ResultadoConsenso escogerPorTamanoObjetivo(const std::vector<ResultadoConsenso>& candidatos, int l) {
    return *std::min_element(candidatos.begin(), candidatos.end(),
        [l](const ResultadoConsenso& a, const ResultadoConsenso& b) {
            int diffA = std::abs(static_cast<int>(a.superstring.size()) - l);
            int diffB = std::abs(static_cast<int>(b.superstring.size()) - l);
            return diffA < diffB;
        });
}

std::vector<ResultadoConsenso> generarCandidatosPorOrientacion(const std::vector<std::string>& fragmentosOriginales) {
    int n = static_cast<int>(fragmentosOriginales.size());
    std::vector<ResultadoConsenso> candidatos;

    for (int mascara = 0; mascara < (1 << n); ++mascara) {
        std::vector<Fragmento> fragmentos;
        fragmentos.reserve(n);
        for (int i = 0; i < n; ++i) {
            bool usarReverso = (mascara >> i) & 1;
            std::string id = "f" + std::to_string(i + 1) + (usarReverso ? "'" : "");
            std::string secuencia = usarReverso ? complementoReverso(fragmentosOriginales[i]) : fragmentosOriginales[i];
            fragmentos.push_back({id, secuencia, usarReverso});
        }

        auto pesos = construirMultigrafoOverlap(fragmentos);
        candidatos.push_back(ensamblarPorGreedy(fragmentos, pesos));
    }

    return candidatos;
}
