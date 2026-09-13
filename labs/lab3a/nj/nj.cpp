#include "nj.hpp"
#include <limits>
#include <stdexcept>

using namespace std;

std::shared_ptr<NodoNJ> neighborJoining(std::vector<std::vector<double>> matriz,
                                         std::vector<std::string> nombres) {
    vector<shared_ptr<NodoNJ>> clusters;
    for (const auto& nom : nombres) {
        clusters.push_back(make_shared<NodoNJ>(nom));
    }

    while (clusters.size() > 2) {
        int n = matriz.size();

        vector<double> u(n, 0.0);
        for (int i = 0; i < n; i++) {
            double suma = 0.0;
            for (int k = 0; k < n; k++) suma += matriz[i][k];
            u[i] = suma / (n - 2);
        }

        double mejor = numeric_limits<double>::infinity();
        int mi = -1, mj = -1;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                double val = matriz[i][j] - u[i] - u[j];
                if (val < mejor) {
                    mejor = val;
                    mi = i;
                    mj = j;
                }
            }
        }

        if (mi == -1 || mj == -1) {
            throw runtime_error("neighborJoining: no se encontro par valido (matriz de distancias degenerada)");
        }

        double dIij = 0.5 * (matriz[mi][mj] + u[mi] - u[mj]);
        double dJij = 0.5 * (matriz[mi][mj] + u[mj] - u[mi]);

        auto nuevo = make_shared<NodoNJ>("(" + clusters[mi]->nombre + "," + clusters[mj]->nombre + ")");
        nuevo->izquierdo = clusters[mi];
        nuevo->longIzquierdo = dIij;
        nuevo->derecho = clusters[mj];
        nuevo->longDerecho = dJij;

        vector<double> nuevaFila;
        for (int k = 0; k < n; k++) {
            if (k == mi || k == mj) continue;
            double d = (matriz[mi][k] + matriz[mj][k] - matriz[mi][mj]) / 2.0;
            nuevaFila.push_back(d);
        }

        vector<shared_ptr<NodoNJ>> clustersNuevos;
        for (int k = 0; k < n; k++) {
            if (k == mi || k == mj) continue;
            clustersNuevos.push_back(clusters[k]);
        }
        clustersNuevos.push_back(nuevo);

        int nuevoTotal = clustersNuevos.size();
        vector<vector<double>> matrizNueva(nuevoTotal, vector<double>(nuevoTotal, 0.0));

        for (int a = 0; a < nuevoTotal - 1; a++) {
            for (int b = 0; b < nuevoTotal - 1; b++) {
                int origA = a, origB = b;
                if (a >= mi) origA++;
                if (origA >= mj) origA++;
                if (b >= mi) origB++;
                if (origB >= mj) origB++;
                matrizNueva[a][b] = matriz[origA][origB];
            }
        }
        for (int a = 0; a < nuevoTotal - 1; a++) {
            matrizNueva[a][nuevoTotal - 1] = nuevaFila[a];
            matrizNueva[nuevoTotal - 1][a] = nuevaFila[a];
        }

        matriz = matrizNueva;
        clusters = clustersNuevos;
    }

    auto raiz = make_shared<NodoNJ>("(" + clusters[0]->nombre + "," + clusters[1]->nombre + ")");
    raiz->izquierdo = clusters[0];
    raiz->longIzquierdo = matriz[0][1];
    raiz->derecho = clusters[1];
    raiz->longDerecho = 0.0;

    return raiz;
}
