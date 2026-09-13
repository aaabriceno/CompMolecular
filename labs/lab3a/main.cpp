#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "matriz/matriz.hpp"
#include "upgma/upgma.hpp"
#include "tree/newick.hpp"
#include "nj/nj.hpp"
#include "tree/newick_nj.hpp"

using namespace std;

int main(){
    vector<string> secuencias = {
        "ATTGCCATT",   // S1
        "ATGGCCATT",   // S2
        "ATCCAATTTT",  // S3
        "ATCTTTCTT",   // S4
        "ACTGACC"      // S5
    };

    vector<vector<double>> M = construirMatrizDistancias(secuencias);

    int n = M.size();
    cout << fixed << setprecision(4);
    cout <<"\t";
    
    for (int j = 0; j < n; j++){
        cout << "S" << (j+1) << "\t";
    }
    cout << "\n";

    for (int i = 0; i < n;i++){
        cout << "S" << (i+1) << "\t";
        for (int j = 0; j < n; j++){
            cout << M[i][j] << "\t";
        }
        cout << "\n";
    }

    vector<string> nombres = {"S1", "S2", "S3", "S4", "S5"};
    auto raizUpgma = upgma(M, nombres);

    cout << "\nArbol UPGMA (Newick):\n";
    cout << paNewick(raizUpgma) << "\n";

    generarDot(raizUpgma, "upgma_tree.dot");
    cout << "\nArchivo upgma_tree.dot generado.\n";
    cout << "Renderizar con: dot -Tpng upgma_tree.dot -o upgma_tree.png\n";

    auto raizNj = neighborJoining(M, nombres);

    cout << "\nArbol Neighbor Joining (Newick):\n";
    cout << paNewickNJ(raizNj) << "\n";

    generarDotNJ(raizNj, "nj_tree.dot");
    cout << "\nArchivo nj_tree.dot generado.\n";
    cout << "Renderizar con: dot -Tpng nj_tree.dot -o nj_tree.png\n";

    return 0;
}