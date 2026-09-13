#include "newick.hpp"
#include <fstream>
#include <sstream>

using namespace std;

static string nodoNewickRec(shared_ptr<NodoUPGMA> nodo, double alturaPadre) {
    double longitudRama = alturaPadre - nodo->altura;

    ostringstream oss;
    if (!nodo->izquierdo && !nodo->derecho) {
        oss << nodo->nombre << ":" << longitudRama;
        return oss.str();
    }

    oss << "(" << nodoNewickRec(nodo->izquierdo, nodo->altura)
        << "," << nodoNewickRec(nodo->derecho, nodo->altura) << ")";
    oss << ":" << longitudRama;
    return oss.str();
}

string paNewick(shared_ptr<NodoUPGMA> nodo) {
    if (!nodo->izquierdo && !nodo->derecho) {
        return nodo->nombre + ";";
    }
    ostringstream oss;
    oss << "(" << nodoNewickRec(nodo->izquierdo, nodo->altura)
        << "," << nodoNewickRec(nodo->derecho, nodo->altura) << ");";
    return oss.str();
}

static void dotRec(shared_ptr<NodoUPGMA> nodo, ostream& out, int& contador, string& idPropio) {
    if (!nodo->izquierdo && !nodo->derecho) {
        idPropio = nodo->nombre;
        out << "  \"" << idPropio << "\" [label=\"" << nodo->nombre << "\", shape=box];\n";
        return;
    }

    idPropio = "interno" + to_string(contador++);
    out << "  \"" << idPropio << "\" [label=\"" << nodo->altura << "\", shape=circle];\n";

    string idIzq, idDer;
    dotRec(nodo->izquierdo, out, contador, idIzq);
    dotRec(nodo->derecho, out, contador, idDer);

    out << "  \"" << idPropio << "\" -- \"" << idIzq << "\";\n";
    out << "  \"" << idPropio << "\" -- \"" << idDer << "\";\n";
}

void generarDot(shared_ptr<NodoUPGMA> nodo, const string& rutaArchivo) {
    ofstream out(rutaArchivo);
    out << "graph UPGMA {\n";
    int contador = 0;
    string idRaiz;
    dotRec(nodo, out, contador, idRaiz);
    out << "}\n";
}
