#include "MSAestrella.hpp"
#include "../nw/nw.hpp"
#include <algorithm>
#include <climits>
using namespace std;

vector<vector<int>> matrizScoresPairwise(const vector<string>& seqs){
    int k = seqs.size();
    vector<vector<int>> scores(k, vector<int>(k, 0));

    for(int i = 0; i < k; i++){
        for(int j = i+1; j < k; j++){
            auto M = construirMatriz(seqs[i], seqs[j]);
            int score = M[seqs[i].size()][seqs[j].size()];
            scores[i][j] = score;
            scores[j][i] = score; // se que es simetrico, por eso lo copio a ambos lados
        }
    }
    return scores;
}

int elegirCentro(const vector<vector<int>>& scores){
    int k = scores.size();
    int mejorIndice = 0;
    int mejorSuma = INT_MIN;

    for(int i = 0; i < k; i++){
        int suma = 0;
        for(int j = 0; j < k; j++){
            suma += scores[i][j];
        }
        if(suma > mejorSuma){
            mejorSuma = suma;
            mejorIndice = i;
        }
    }
    return mejorIndice; // me quedo con la secuencia que mas suma score contra las demas
}

static void insertarGapEnTodas(vector<string>& msaParcial, int posicion){
    // aqui inserto una columna de gap en TODAS las secuencias que ya tengo en el msa
    for(auto& s : msaParcial){
        s.insert(s.begin() + posicion, '-');
    }
}

static string alinearSeqAlMSA(vector<string>& msaParcial, const string& centroViejo,
                               const string& centroNuevo, const string& seqAlineada){
    // recorro centroViejo y centroNuevo en paralelo. donde centroNuevo tiene un
    // gap que centroViejo no tenia, es porque este nuevo alineamiento pairwise
    // metio un gap nuevo en el centro, entonces tengo que propagar ese gap a
    // todo lo que ya tenia armado en el msa (para no romper las columnas)
    string seqFinal = "";
    size_t iViejo = 0, iNuevo = 0;
    int columna = 0;

    while(iNuevo < centroNuevo.size()){
        bool gapNuevoNoExistiaAntes =
            (centroNuevo[iNuevo] == '-') &&
            (iViejo >= centroViejo.size() || centroViejo[iViejo] != '-');

        if(gapNuevoNoExistiaAntes){
            insertarGapEnTodas(msaParcial, columna);
            seqFinal += seqAlineada[iNuevo];
            columna++;
            iNuevo++;
        } else {
            seqFinal += seqAlineada[iNuevo];
            columna++;
            iNuevo++;
            iViejo++;
        }
    }
    return seqFinal;
}

vector<ResultadoMSA> msaEstrella(const vector<string>& seqs){
    int k = seqs.size();
    auto scores = matrizScoresPairwise(seqs);
    int centro = elegirCentro(scores);

    vector<string> msaParcial;
    msaParcial.push_back(seqs[centro]); // arranco el msa solo con el centro, sin gaps

    // guardo en que indice original quedo cada fila de msaParcial, para poder
    // reordenar al final y que calce con el orden en que yo pase las seqs
    vector<int> indiceOriginal;
    indiceOriginal.push_back(centro);

    for(int i = 0; i < k; i++){
        if(i == centro) continue;

        string centroViejo = msaParcial[0]; // guardo como esta el centro antes de este paso
        auto M = construirMatriz(centroViejo, seqs[i]);
        auto par = tracebackUnico(M, centroViejo, seqs[i]);
        string centroNuevo = par.first;
        string seqAlineada = par.second;

        // fusiono este alineamiento pairwise dentro del msa que ya tengo
        string seqFinal = alinearSeqAlMSA(msaParcial, centroViejo, centroNuevo, seqAlineada);
        msaParcial.push_back(seqFinal);
        indiceOriginal.push_back(i);
    }

    // reordeno msaParcial para que quede en el mismo orden en que yo pase las seqs
    vector<string> msaOrdenado(k);
    for(int pos = 0; pos < k; pos++){
        msaOrdenado[indiceOriginal[pos]] = msaParcial[pos];
    }

    ResultadoMSA resultado;
    resultado.alineamiento = msaOrdenado;
    resultado.scoreSP = scoreSumaDePares(msaOrdenado);

    return {resultado};
}

// aplico el merge de un par (centroNuevo, seqAlineada) sobre un msa parcial que ya tengo,
// y devuelvo una copia nueva del msa con la secuencia fusionada agregada al final
static vector<string> fusionarUnaSecuencia(const vector<string>& msaParcial,
                                            const string& centroViejo,
                                            const string& centroNuevo,
                                            const string& seqAlineada){
    vector<string> copia = msaParcial;
    string seqFinal = alinearSeqAlMSA(copia, centroViejo, centroNuevo, seqAlineada);
    copia.push_back(seqFinal);
    return copia;
}

vector<ResultadoMSA> msaEstrellaTodasLasSoluciones(const vector<string>& seqs){
    int k = seqs.size();
    auto scores = matrizScoresPairwise(seqs);
    int centro = elegirCentro(scores);

    // por cada secuencia distinta al centro, obtengo TODAS las soluciones
    // optimas de alinearla contra el centro (empates incluidos)
    vector<int> indices; // en que orden voy a ir fusionando (todo menos el centro)
    vector<vector<pair<string,string>>> solucionesPorSecuencia;

    for(int i = 0; i < k; i++){
        if(i == centro) continue;
        auto M = construirMatriz(seqs[centro], seqs[i]);
        vector<pair<string,string>> soluciones;
        traceback(seqs[centro].size(), seqs[i].size(), "", "", M, seqs[centro], seqs[i], soluciones);
        solucionesPorSecuencia.push_back(soluciones);
        indices.push_back(i);
    }

    // arranco con un solo msa parcial: el centro solo, sin gaps
    vector<vector<string>> msasParciales;
    msasParciales.push_back({seqs[centro]});

    // por cada secuencia, cruzo cada msa parcial que tengo con cada solucion
    // posible de esa secuencia contra el centro (producto cartesiano)
    for(size_t s = 0; s < solucionesPorSecuencia.size(); s++){
        vector<vector<string>> nuevosMsasParciales;

        for(auto& msaParcial : msasParciales){
            string centroViejo = msaParcial[0];
            for(auto& sol : solucionesPorSecuencia[s]){
                string centroNuevo = sol.first;
                string seqAlineada = sol.second;
                nuevosMsasParciales.push_back(
                    fusionarUnaSecuencia(msaParcial, centroViejo, centroNuevo, seqAlineada)
                );
            }
        }

        msasParciales = nuevosMsasParciales;
    }

    // reordeno cada msa final al orden original de seqs, y calculo su score SP
    vector<int> indiceOriginal;
    indiceOriginal.push_back(centro);
    for(int i : indices) indiceOriginal.push_back(i);

    vector<ResultadoMSA> resultados;
    for(auto& msaParcial : msasParciales){
        vector<string> msaOrdenado(k);
        for(int pos = 0; pos < k; pos++){
            msaOrdenado[indiceOriginal[pos]] = msaParcial[pos];
        }
        ResultadoMSA r;
        r.alineamiento = msaOrdenado;
        r.scoreSP = scoreSumaDePares(msaOrdenado);
        resultados.push_back(r);
    }
    return resultados;
}

int scoreSumaDePares(const MSA& msa){
    int k = msa.size();
    int total = 0;
    int L = msa[0].size();

    for(int i = 0; i < k; i++){
        for(int j = i+1; j < k; j++){
            for(int c = 0; c < L; c++){
                char a = msa[i][c];
                char b = msa[j][c];
                if(a == '-' && b == '-') continue; // dos gaps juntos no suman ni restan
                if(a == '-' || b == '-') total += GAP;
                else total += (a == b ? MATCH : MISMATCH);
            }
        }
    }
    return total;
}
