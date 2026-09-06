#ifndef DOTPLOT_H
#define DOTPLOT_H

#include <string>

// Genera un dot plot (matriz de puntos) entre s y t y lo guarda como imagen PGM.
// Usa una ventana deslizante de tamano `ventana`: para cada posicion (i,j) cuenta
// cuantos caracteres coinciden en esa ventana; si el conteo alcanza `umbral`,
// el pixel se pinta oscuro (region similar), si no, claro.
void generarDotPlot(const std::string& s, const std::string& t,
                     const std::string& archivoSalida,
                     int ventana = 10, int umbral = 7);

#endif
