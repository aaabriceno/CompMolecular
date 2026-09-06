#include <iostream>
#include "parser.h"
#include "nw.h"
#include "dotplot.h"
using namespace std;

int main(){
    vector<pair<string,string>> secuencias = leerSecuencias("Sequencias.txt");

    cout << "Secuencias leidas: " << secuencias.size() << endl;
    for(auto& sec : secuencias){
        cout << " - " << sec.first << ": " << sec.second.size() << " nucleotidos" << endl;
    }

    // Alinear cada par posible (combinaciones de 2 de las 3 secuencias)
    for(size_t a = 0; a < secuencias.size(); a++){
        for(size_t b = a+1; b < secuencias.size(); b++){
            const string& nombreA = secuencias[a].first;
            const string& nombreB = secuencias[b].first;
            const string& s = secuencias[a].second;
            const string& t = secuencias[b].second;

            cout << "\n=== Alineando " << nombreA << " vs " << nombreB << " ===" << endl;

            vector<vector<int>> M = construirMatriz(s, t);
            int n = s.size();
            int m = t.size();

            cout << "Score optimo: " << M[n][m] << endl;

            // Con cadenas grandes hay demasiadas soluciones optimas empatadas
            // para enumerarlas todas (PDF, seccion 2.2): usamos tracebackUnico,
            // que reconstruye una sola solucion optima minimizando rupturas.
            pair<string,string> solucion = tracebackUnico(M, s, t);

            cout << "Primeros 80 caracteres de la solucion optima (menos rupturas):" << endl;
            cout << solucion.first.substr(0, 80) << endl;
            cout << solucion.second.substr(0, 80) << endl;

            string archivoDotPlot = "dotplot_" + nombreA + "_vs_" + nombreB + ".pgm";
            generarDotPlot(s, t, archivoDotPlot);
            cout << "Dot plot guardado en: " << archivoDotPlot << endl;
        }
    }

    return 0;
}
