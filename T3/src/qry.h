#ifndef LIB_QRY_H
#define LIB_QRY_H

#include "srbTree.h"

/* Realiza todas as operações de leitura e interpretação dos arquivos .qry */
void qry_parser(char* BED, char* BSD, char* geo_name, char* qry_name, SRBTree shapes);

#endif