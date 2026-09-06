#include "parserBRCA1.hpp"
#include <fstream>
#include <cctype>
using namespace std;

vector<pair<string,string>> leerBRCA1(const string& archivoTXT){
    ifstream in(archivoTXT);
    vector<pair<string,string>> resultado;
    string linea;
    string muestraActual = "";

    while(getline(in, linea)){
        // detectar si la linea inicia una nueva muestra: primer caracter no-espacio es letra
        // seguida de espacio y luego "BRCA1" (ej: "A BRCA1  F: ...")
        size_t k = 0;
        while(k < linea.size() && isspace((unsigned char)linea[k])) k++;
        if(k >= linea.size()) continue; // linea vacia

        // una linea de nueva muestra trae "BRCA1" (ej "F BRCA1  F: ..."), mientras
        // que una linea de continuacion es solo la etiqueta indentada (ej "         R: ...")
        // no puedo distinguir solo por linea[k]=='F' porque la muestra F choca con la
        // etiqueta "F:" de forward
        bool esLineaDeNuevaMuestra = (linea.find("BRCA1") != string::npos);

        if(esLineaDeNuevaMuestra){
            muestraActual = string(1, linea[k]); // "A", "B", "C", ...
        }

        // buscar "F:" o "R:" en la linea
        char strand = 0;
        size_t posEtiqueta = linea.find("F:");
        if(posEtiqueta != string::npos){
            strand = 'F';
        } else {
            posEtiqueta = linea.find("R:");
            if(posEtiqueta != string::npos) strand = 'R';
        }
        if(strand == 0) continue; // no trae secuencia esta linea

        // extraemos solo letras ACGT (ignoramos 5', -3', comillas unicode, espacios)
        string secuencia = "";
        for(size_t i = posEtiqueta; i < linea.size(); i++){
            char c = linea[i];
            if(isalpha((unsigned char)c)){
                char up = toupper(c);
                if(up=='A'||up=='C'||up=='G'||up=='T'){
                    secuencia += up;
                }
            }
        }

        string nombre = muestraActual + "_" + string(1, strand);
        resultado.push_back({nombre, secuencia});
    }
    return resultado;
}
