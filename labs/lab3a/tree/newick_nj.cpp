#include "newick_nj.hpp"
#include <fstream>
#include <sstream>

using namespace std;

static string nodoNewickRec(shared_ptr<NodoNJ> nodo, double longRama) {
    ostringstream oss;
    if (!nodo->izquierdo && !nodo->derecho) {
        oss << nodo->nombre << ":" << longRama;
        return oss.str();
    }

    oss << "(" << nodoNewickRec(nodo->izquierdo, nodo->longIzquierdo)
        << "," << nodoNewickRec(nodo->derecho, nodo->longDerecho) << ")";
    oss << ":" << longRama;
    return oss.str();
}

string paNewickNJ(shared_ptr<NodoNJ> nodo) {
    if (!nodo->izquierdo && !nodo->derecho) {
        return nodo->nombre + ";";
    }
    ostringstream oss;
    oss << "(" << nodoNewickRec(nodo->izquierdo, nodo->longIzquierdo)
        << "," << nodoNewickRec(nodo->derecho, nodo->longDerecho) << ");";
    return oss.str();
}

static void dotRec(shared_ptr<NodoNJ> nodo, ostream& out, int& contador, string& idPropio) {
    if (!nodo->izquierdo && !nodo->derecho) {
        idPropio = nodo->nombre;
        out << "  \"" << idPropio << "\" [label=\"" << nodo->nombre << "\", shape=box];\n";
        return;
    }

    idPropio = "interno" + to_string(contador++);
    out << "  \"" << idPropio << "\" [label=\"\", shape=point];\n";

    string idIzq, idDer;
    dotRec(nodo->izquierdo, out, contador, idIzq);
    dotRec(nodo->derecho, out, contador, idDer);

    out << "  \"" << idPropio << "\" -- \"" << idIzq << "\" [label=\"" << nodo->longIzquierdo << "\"];\n";
    out << "  \"" << idPropio << "\" -- \"" << idDer << "\" [label=\"" << nodo->longDerecho << "\"];\n";
}

void generarDotNJ(shared_ptr<NodoNJ> nodo, const string& rutaArchivo) {
    ofstream out(rutaArchivo);
    out << "graph NJ {\n";
    int contador = 0;
    string idRaiz;
    dotRec(nodo, out, contador, idRaiz);
    out << "}\n";
}
