#ifndef LIB_GEO_H
#define LIB_GEO_H

#include "srbTree.h"

/* Realiza todas as operações de leitura e interpretação dos arquivos .geo */
void geo_parser(char* BED, char* BSD, char* geo_name, SRBTree shapes);

#endif