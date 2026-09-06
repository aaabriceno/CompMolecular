#include <iostream>
#include <chrono>
#include <set>
#include "nw/nw.hpp"
#include "parser/parser.hpp"
#include "parser/parserBRCA1.hpp"
#include "msa/MSAestrella.hpp"
using namespace std;

// aqui imprimo un msa alineado, una secuencia por linea, con su nombre
static void imprimirMSA(const vector<string>& nombres, const MSA& msa){
    for(size_t i = 0; i < msa.size(); i++){
        cout << nombres[i] << ": " << msa[i] << "\n";
    }
}

// concateno el msa en un solo string para poder detectar combinaciones repetidas
static string aplanarMSA(const MSA& msa){
    string plano = "";
    for(auto& s : msa) plano += s + "|";
    return plano;
}

int main(){
    // ---- prueba 1: ejemplo visto en clase (S1..S5), con TODAS las soluciones optimas ----
    vector<string> nombresClase = {"S1","S2","S3","S4","S5"};
    vector<string> seqsClase = {
        "ATTGCCATT",
        "ATGGCCATT",
        "ATCCAATTTT",
        "ATCTTCTT",
        "ACTGACC"
    };

    cout << "=== Ejemplo de clase (S1-S5): todas las soluciones ===\n";
    auto resultadosClase = msaEstrellaTodasLasSoluciones(seqsClase);

    // como el producto cartesiano puede repetir el mismo msa final por caminos
    // distintos, filtro duplicados antes de mostrar
    set<string> vistos;
    int contadorUnicos = 0;
    for(auto& r : resultadosClase){
        string plano = aplanarMSA(r.alineamiento);
        if(vistos.count(plano)) continue;
        vistos.insert(plano);
        contadorUnicos++;

        cout << "-- Solucion " << contadorUnicos << " --\n";
        imprimirMSA(nombresClase, r.alineamiento);
        cout << "Score SP: " << r.scoreSP << "\n\n";
    }
    cout << "Total de soluciones optimas distintas encontradas: " << contadorUnicos << "\n\n";

    // ---- prueba 2: BRCA1, separando forward y reverse ----
    auto muestrasBRCA1 = leerBRCA1("BRCA1.txt");

    vector<string> nombresF, seqsF, nombresR, seqsR;
    for(auto& par : muestrasBRCA1){
        // el nombre viene como "A_F" o "A_R", reviso el ultimo caracter
        if(par.first.back() == 'F'){
            nombresF.push_back(par.first);
            seqsF.push_back(par.second);
        } else {
            nombresR.push_back(par.first);
            seqsR.push_back(par.second);
        }
    }

    cout << "=== BRCA1: solo forward ===\n";
    auto resultadoF = msaEstrella(seqsF);
    for(auto& r : resultadoF){
        imprimirMSA(nombresF, r.alineamiento);
        cout << "Score SP: " << r.scoreSP << "\n\n";
    }

    cout << "=== BRCA1: solo reverse ===\n";
    auto resultadoR = msaEstrella(seqsR);
    for(auto& r : resultadoR){
        imprimirMSA(nombresR, r.alineamiento);
        cout << "Score SP: " << r.scoreSP << "\n\n";
    }

    // ---- prueba 3: impacto de cantidad de cadenas vs longitud de cadenas ----
    // genero cadenas aleatorias de ADN para medir tiempo de ejecucion de msaEstrella
    // variando por separado: (a) cantidad de secuencias con longitud fija,
    // (b) longitud de secuencia con cantidad fija. asi comparo cual factor pesa mas.
    auto generarSecuenciaAleatoria = [](int longitud, unsigned semilla) -> string {
        string bases = "ACGT";
        string seq = "";
        unsigned estado = semilla;
        for(int i = 0; i < longitud; i++){
            estado = estado * 1103515245u + 12345u; // generador simple, reproducible
            seq += bases[(estado / 65536) % 4];
        }
        return seq;
    };

    cout << "=== Impacto de la CANTIDAD de cadenas (longitud fija = 50) ===\n";
    for(int cantidad : {3, 5, 8, 12, 16}){
        vector<string> seqsPrueba;
        for(int i = 0; i < cantidad; i++){
            seqsPrueba.push_back(generarSecuenciaAleatoria(50, i+1));
        }
        auto inicio = chrono::high_resolution_clock::now();
        auto resultado = msaEstrella(seqsPrueba);
        auto fin = chrono::high_resolution_clock::now();
        double ms = chrono::duration<double, milli>(fin - inicio).count();
        cout << "cantidad=" << cantidad << " -> tiempo=" << ms << " ms, scoreSP="
             << resultado[0].scoreSP << "\n";
    }
    cout << "\n";

    cout << "=== Impacto de la LONGITUD de cadenas (cantidad fija = 6) ===\n";
    for(int longitud : {20, 50, 100, 200, 400}){
        vector<string> seqsPrueba;
        for(int i = 0; i < 6; i++){
            seqsPrueba.push_back(generarSecuenciaAleatoria(longitud, i+1));
        }
        auto inicio = chrono::high_resolution_clock::now();
        auto resultado = msaEstrella(seqsPrueba);
        auto fin = chrono::high_resolution_clock::now();
        double ms = chrono::duration<double, milli>(fin - inicio).count();
        cout << "longitud=" << longitud << " -> tiempo=" << ms << " ms, scoreSP="
             << resultado[0].scoreSP << "\n";
    }

    return 0;
}
