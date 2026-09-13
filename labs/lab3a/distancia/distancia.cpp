#include <limits>
#include <cmath>
#include <vector>
#include "../nw/nw.hpp"
using namespace std;

//funcion de distancias de Jukes Cantor basado en lo que se explica en los pdfs del curso
double distanciaJukesCantor(const string& a,const string& b ){
    vector<vector<int>> M = construirMatriz(a,b);
    pair<string,string> alineado = tracebackUnico(M,a,b);

    int diferencias = 0;
    int total = alineado.first.size();

    for (int i = 0; i < total; i++){
        if (alineado.first[i] != alineado.second[i]){
            diferencias++;
        }
    }

    double p = (double) diferencias / total;

    if (p >= 0.75){
        return numeric_limits<double>::infinity();
    }

    //formula matemática del modelo de Jukes Cantor
    double respuesta = -0.75 * log(1- (4.0/3.0) *p);
    return respuesta;
}

// distancia sin correccion Jukes-Cantor: fraccion simple de diferencias tras alinear con NW.
// util para texto libre (ej. apellidos) donde JC no aplica (asume alfabeto de 4 simbolos ADN).
double distanciaFraccionDiferencias(const string& a, const string& b){
    vector<vector<int>> M = construirMatriz(a,b);
    pair<string,string> alineado = tracebackUnico(M,a,b);

    int diferencias = 0;
    int total = alineado.first.size();

    for (int i = 0; i < total; i++){
        if (alineado.first[i] != alineado.second[i]){
            diferencias++;
        }
    }

    return (double) diferencias / total;
}