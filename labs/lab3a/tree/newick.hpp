#ifndef NEWICK_HPP
#define NEWICK_HPP

#include <string>
#include <memory>
#include "../upgma/upgma.hpp"

std::string paNewick(std::shared_ptr<NodoUPGMA> nodo);

void generarDot(std::shared_ptr<NodoUPGMA> nodo, const std::string& rutaArchivo);

#endif
