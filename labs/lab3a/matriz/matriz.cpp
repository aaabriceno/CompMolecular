#include "matriz.hpp"
#include "../distancia/distancia.hpp"
using namespace std;

vector<vector<double>> construirMatrizDistancias(const vector<string>& secuencias){
    return construirMatrizDistancias(secuencias, distanciaJukesCantor);
}

vector<vector<double>> construirMatrizDistancias(
    const vector<string>& secuencias,
    function<double(const string&, const string&)> funcionDistancia){

    int n = secuencias.size();
    vector<vector<double>> matrizN(n, vector<double>(n, 0.0));

    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){
            double d = funcionDistancia(secuencias[i], secuencias[j]);
            matrizN[i][j] = d;
            matrizN[j][i] = d;
        }
    }

    return matrizN;
}
