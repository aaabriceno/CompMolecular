#include "dotplot.h"
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;

void generarDotPlot(const string& s, const string& t,
                     const string& archivoSalida,
                     int ventana, int umbral){

    int n = s.size();
    int m = t.size();

    // Downsample: si la secuencia es grande, no comparamos cada posicion,
    // saltamos de a `paso` para que la imagen final no pase de ~500x500 px.
    const int MAX_LADO = 500;
    int pasoI = max(1, n / MAX_LADO);
    int pasoJ = max(1, m / MAX_LADO);

    int filas = (n / pasoI);
    int cols  = (m / pasoJ);

    vector<vector<unsigned char>> imagen(filas, vector<unsigned char>(cols, 255));

    for(int fi = 0; fi < filas; fi++){
        int i = fi * pasoI;
        if(i + ventana > n) continue;

        for(int fj = 0; fj < cols; fj++){
            int j = fj * pasoJ;
            if(j + ventana > m) continue;

            int coincidencias = 0;
            for(int k = 0; k < ventana; k++){
                if(s[i+k] == t[j+k]) coincidencias++;
            }

            if(coincidencias >= umbral){
                // si hay mas coincidencias el pixel sera pintado mas oscuro
                int intensidad = 255 - (coincidencias * 255 / ventana);
                imagen[fi][fj] = (unsigned char) intensidad;
            }
        }
    }

    ofstream out(archivoSalida);
    out << "P2\n" << cols << " " << filas << "\n255\n";
    for(int fi = 0; fi < filas; fi++){
        for(int fj = 0; fj < cols; fj++){
            out << (int) imagen[fi][fj] << " ";
        }
        out << "\n";
    }
}
