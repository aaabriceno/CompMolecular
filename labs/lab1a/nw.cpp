#include "nw.h"
#include <algorithm>
using namespace std;

vector<vector<int>> construirMatriz(const string& s, const string& t){
    int n = s.size();
    int m = t.size();

    vector<vector<int>> M(n+1, vector<int>(m+1, 0));

    for(int i = 0; i <= n; i++) M[i][0] = i * GAP;
    for(int j = 0; j <= m; j++) M[0][j] = j * GAP;

    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= m; j++){
            int diag = M[i-1][j-1] + (s[i-1] == t[j-1] ? MATCH : MISMATCH);
            int up   = M[i-1][j] + GAP;
            int left = M[i][j-1] + GAP;
            M[i][j] = max({diag, up, left});
        }
    }
    return M;
}

// Reconstruye, por backtracking, TODAS las soluciones optimas de alineamiento.
void traceback(int i, int j, string alignS, string alignT,
                vector<vector<int>>& M, const string& s, const string& t,
                vector<pair<string,string>>& soluciones){

    if(i == 0 && j == 0){
        reverse(alignS.begin(), alignS.end());
        reverse(alignT.begin(), alignT.end());
        soluciones.push_back({alignS, alignT});
        return;
    }

    if(i > 0 && j > 0){
        int score = (s[i-1] == t[j-1]) ? MATCH : MISMATCH;
        if(M[i][j] == M[i-1][j-1] + score){
            traceback(i-1, j-1, alignS + s[i-1], alignT + t[j-1], M, s, t, soluciones);
        }
    }
    if(i > 0){
        if(M[i][j] == M[i-1][j] + GAP){
            traceback(i-1, j, alignS + s[i-1], alignT + '-', M, s, t, soluciones);
        }
    }
    if(j > 0){
        if(M[i][j] == M[i][j-1] + GAP){
            traceback(i, j-1, alignS + '-', alignT + t[j-1], M, s, t, soluciones);
        }
    }
}

pair<string,string> tracebackUnico(vector<vector<int>>& M, const string& s, const string& t){
    int i = s.size();
    int j = t.size();
    string alignS, alignT;

    // 0 = ninguna todavia, 1 = veniamos de arriba (gap en t), 2 = veniamos de izquierda (gap en s)
    int direccionAnterior = 0;

    while(i > 0 || j > 0){
        bool puedeDiag = (i > 0 && j > 0) &&
            (M[i][j] == M[i-1][j-1] + (s[i-1] == t[j-1] ? MATCH : MISMATCH));
        bool puedeArriba = (i > 0) && (M[i][j] == M[i-1][j] + GAP);
        bool puedeIzquierda = (j > 0) && (M[i][j] == M[i][j-1] + GAP);

        if(puedeDiag){
            alignS += s[i-1];
            alignT += t[j-1];
            i--; j--;
            direccionAnterior = 0;
        } else if(puedeArriba && direccionAnterior == 1){
            // seguimos el mismo bloque de gaps ya abierto: no crea ruptura nueva
            alignS += s[i-1];
            alignT += '-';
            i--;
        } else if(puedeIzquierda && direccionAnterior == 2){
            alignS += '-';
            alignT += t[j-1];
            j--;
        } else if(puedeArriba){
            alignS += s[i-1];
            alignT += '-';
            i--;
            direccionAnterior = 1;
        } else { // puedeIzquierda
            alignS += '-';
            alignT += t[j-1];
            j--;
            direccionAnterior = 2;
        }
    }

    reverse(alignS.begin(), alignS.end());
    reverse(alignT.begin(), alignT.end());
    return {alignS, alignT};
}
