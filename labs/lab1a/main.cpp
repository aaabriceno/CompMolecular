#include <iostream>
#include "nw.h"
using namespace std;

int main(){
    string s = "AAAC";
    string t = "AGC";

    vector<vector<int>> M = construirMatriz(s, t);
    int n = s.size();
    int m = t.size();

    cout << "Matriz M:" << endl;
    for(int i = 0; i <= n; i++){
        for(int j = 0; j <= m; j++){
            cout << M[i][j] << "\t";
        }
        cout << endl;
    }

    cout << "\nScore optimo: " << M[n][m] << endl;

    vector<pair<string,string>> soluciones;
    traceback(n, m, "", "", M, s, t, soluciones);

    cout << "\nSoluciones optimas encontradas: " << soluciones.size() << endl;
    for(size_t k = 0; k < soluciones.size(); k++){
        cout << "Solucion " << k+1 << ":" << endl;
        cout << soluciones[k].first << endl;
        cout << soluciones[k].second << endl;
    }

    return 0;
}
