#ifndef NEWICK_NJ_HPP
#define NEWICK_NJ_HPP

#include <string>
#include <memory>
#include "../nj/nj.hpp"

std::string paNewickNJ(std::shared_ptr<NodoNJ> nodo);

void generarDotNJ(std::shared_ptr<NodoNJ> nodo, const std::string& rutaArchivo);

#endif
