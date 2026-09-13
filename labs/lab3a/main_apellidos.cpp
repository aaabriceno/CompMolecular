#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "matriz/matriz.hpp"
#include "distancia/distancia.hpp"
#include "upgma/upgma.hpp"
#include "tree/newick.hpp"
#include "nj/nj.hpp"
#include "tree/newick_nj.hpp"

using namespace std;

int main(){
    vector<string> personas = {
        "BECERRARODRIGUEZ",   // Paul Ignacio
        "BRICENOQUIROZ",      // Anthony Angel
        "CRUZLAURA",          // Eduardo Jacob
        "DIAZNEYRA",          // Gino Sebastian
        "GORDILLOYUCRA",      // Felix Daniel
        "QUISPEARROYO",       // Angel Jesus
        "RAMIREZURDAY",       // Diego Alonso
        "SALAZARZUNIGA",      // Camila Valentina
        "SUCAJARAMILLO",      // Bryan Edward
        "TORRESACUNA",        // Marcelo
        "TUPACVALDIVIA"       // Yvan Jesus (profesor)
    };
    vector<string> nombres = {
        "Paul_Becerra", "Anthony_Briceno", "Eduardo_Cruz", "Gino_Diaz",
        "Felix_Gordillo", "Angel_Quispe", "Diego_Ramirez", "Camila_Salazar",
        "Bryan_Suca", "Marcelo_Torres", "Yvan_Tupac"
    };

    vector<vector<double>> M = construirMatrizDistancias(personas, distanciaFraccionDiferencias);

    int n = M.size();
    cout << fixed << setprecision(4);
    cout << "Leyenda:\n";
    for (int i = 0; i < n; i++) {
        cout << nombres[i] << " = " << personas[i] << "\n";
    }

    cout << "\n\t";
    for (int j = 0; j < n; j++){
        cout << nombres[j] << "\t";
    }
    cout << "\n";

    for (int i = 0; i < n; i++){
        cout << nombres[i] << "\t";
        for (int j = 0; j < n; j++){
            cout << M[i][j] << "\t";
        }
        cout << "\n";
    }

    auto raizUpgma = upgma(M, nombres);
    cout << "\nArbol UPGMA (Newick):\n";
    cout << paNewick(raizUpgma) << "\n";
    generarDot(raizUpgma, "upgma_apellidos.dot");
    cout << "Renderizar con: dot -Tpng upgma_apellidos.dot -o upgma_apellidos.png\n";

    auto raizNj = neighborJoining(M, nombres);
    cout << "\nArbol Neighbor Joining (Newick):\n";
    cout << paNewickNJ(raizNj) << "\n";
    generarDotNJ(raizNj, "nj_apellidos.dot");
    cout << "Renderizar con: dot -Tpng nj_apellidos.dot -o nj_apellidos.png\n";

    return 0;
}
