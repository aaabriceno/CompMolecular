#include "hamilton.hpp"

using namespace std;

vector<vector<int>> filtrarPorLinkage (const vector<vector<int>>& pesos, int l){
    size_t n = pesos.size();
    vector<vector<int>> filtrado (n, vector<int>(n,0));
    for (size_t i = 0;i < n; ++i){
        for (size_t j = 0;j < n; ++j){
            if (pesos [i][j] >= l){
                filtrado[i][j] = pesos[i][j];
            }
        }
    }
    return filtrado;
    /*conservamos los pesos mayores o iguales a l, se evaluan 
    mediante el if y se quedan en 0 (sin arista) o se igualan*/
}

namespace {
    bool backtrack(const vector<vector<int>>& pesos, vector<int> &camino, vector<bool> &visitido, int n){
        if (static_cast<int>(camino.size()) == n){
            return true;
            //se visito todos los nodos
        }
        int actual = camino.back();
        for (int siguiente = 0; siguiente < n; ++siguiente){
            if (!visitido[siguiente] && pesos[actual][siguiente] > 0 ){
                visitido[siguiente] = true;
                camino.push_back(siguiente);
                if (backtrack(pesos,camino,visitido,n)){
                    return true;
                }
                camino.pop_back();
                visitido[siguiente] = false;
            }
        }
        return false;
    }

}

vector<int> buscarCaminoHamiltoniano(const vector<vector<int>>&pesos){
    int n = static_cast<int>(pesos.size());
    for (int inicio = 0; inicio < n; ++inicio){
        vector<int> camino = {inicio};
        vector<bool> visitado (n,false);
        visitado[inicio] = true;
        if (backtrack(pesos,camino,visitado,n)){
            return camino;
        }
    }    
    return {}; // no existe un camino Hamiltoniano
}