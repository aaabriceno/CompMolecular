#ifndef PARSERBRCA1_HPP
#define PARSERBRCA1_HPP

#include <string>
#include <vector>


// {nombre, secuencia}, nombre: tipo "A_F", "A_R", "B_F"
std::vector<std::pair<std::string,std::string>> leerBRCA1(const std::string& archivoTXT);

#endif