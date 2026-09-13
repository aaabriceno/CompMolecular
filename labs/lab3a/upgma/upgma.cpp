#include "upgma.hpp"
#include <limits>
#include <stdexcept>

using namespace std;

std::shared_ptr<NodoUPGMA> upgma(std::vector<std::vector<double>> matriz,
                                  std::vector<std::string> nombres) {
    int n = matriz.size();

    vector<shared_ptr<NodoUPGMA>> clusters;
    vector<int> tamanos(n, 1);
    for (int i = 0; i < n; i++) {
        clusters.push_back(make_shared<NodoUPGMA>(nombres[i]));
    }

    while (clusters.size() > 1) {
        int filas = matriz.size();

        double minDist = numeric_limits<double>::infinity();
        int mi = -1, mj = -1;

        for (int i = 0; i < filas; i++) {
            for (int j = i + 1; j < filas; j++) {
                if (matriz[i][j] < minDist) {
                    minDist = matriz[i][j];
                    mi = i;
                    mj = j;
                }
            }
        }

        if (mi == -1 || mj == -1) {
            throw runtime_error("upgma: no se encontro par valido (matriz de distancias con todos los valores infinitos)");
        }

        auto nuevo = make_shared<NodoUPGMA>("(" + clusters[mi]->nombre + "," + clusters[mj]->nombre + ")");
        nuevo->altura = minDist / 2.0;
        nuevo->izquierdo = clusters[mi];
        nuevo->derecho = clusters[mj];

        int tamNuevo = tamanos[mi] + tamanos[mj];

        vector<double> nuevaFila;
        for (int k = 0; k < filas; k++) {
            if (k == mi || k == mj) continue;
            double d = (matriz[mi][k] * tamanos[mi] + matriz[mj][k] * tamanos[mj]) / tamNuevo;
            nuevaFila.push_back(d);
        }

        vector<vector<double>> matrizNueva;
        vector<shared_ptr<NodoUPGMA>> clustersNuevos;
        vector<int> tamanosNuevos;

        for (int k = 0; k < filas; k++) {
            if (k == mi || k == mj) continue;
            clustersNuevos.push_back(clusters[k]);
            tamanosNuevos.push_back(tamanos[k]);
        }
        clustersNuevos.push_back(nuevo);
        tamanosNuevos.push_back(tamNuevo);

        int nuevoTotal = clustersNuevos.size();
        matrizNueva.assign(nuevoTotal, vector<double>(nuevoTotal, 0.0));

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
        tamanos = tamanosNuevos;
    }

    return clusters[0];
}
