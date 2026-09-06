#include "parser.h"
#include <fstream>
#include <cctype>
using namespace std;

bool esLineaDeDatos(const string& linea){
    for (char c: linea){
        if(isdigit((unsigned char)c)){
            return true;
        }
    }
    return false;
}

vector<pair<string,string>> leerSecuencias(const string& archivoTXT){
    ifstream in(archivoTXT);
    //{nombre, secuencia}
    vector<pair<string,string>> resultado; 
    string linea;
    string nombreActual = "";
    string secuenciaActual = "";

    while (getline(in,linea)){
        //linea vacia: ignoramos
        bool vacia = true;
        for (char c : linea){
            if(!isspace((unsigned char)c)){
                vacia = false;
                break;
            }
        }
        if (vacia){
            continue;
        }

        if (esLineaDeDatos(linea)){
            for(char c : linea){
                if (isalpha((unsigned char) c)){
                    //convertimos un caracter dado a su version mayuscula
                    secuenciaActual += toupper(c);
                }
            }
        }
        else{
            if (!nombreActual.empty()){
                resultado.push_back({nombreActual,secuenciaActual});
            }
            // nos quedamos solo con letras y '-' para evitar \r u otros
            // caracteres invisibles que arrastra el archivo de origen
            nombreActual = "";
            for(char c : linea){
                if(isalpha((unsigned char)c) || c == '-'){
                    nombreActual += c;
                }
            }
            secuenciaActual = "";
        }
    }
    if (!nombreActual.empty()){
        resultado.push_back({nombreActual,secuenciaActual});
    }
    return resultado;
}