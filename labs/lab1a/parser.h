#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

bool esLineaDeDatos(const std::string& linea);
std::vector<std::pair<std::string,std::string>> leerSecuencias(const std::string& archivoTXT);

#endif
