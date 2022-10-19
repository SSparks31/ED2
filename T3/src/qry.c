#include "qry.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <linux/limits.h>

#include "list.h"

#include "svg.h"
#include "data.h"


void qry_parser(char* BED, char* BSD, char* geo_name, char* qry_name, SRBTree shapes) {    
    char qry_path[PATH_MAX];
    sprintf(qry_path, "%s%s%s", BED, BED[strlen(BED) - 1] == '/' ? "" : "/", qry_name);
    FILE* qry_file = fopen(qry_path, "r");

    char output_path[PATH_MAX];
    *strrchr(geo_name, '.') = '\0';

    if (strrchr(geo_name, '/')) {
        geo_name = strrchr(geo_name, '/') + 1;
    }
    
    if (strrchr(qry_name, '/')) {
        qry_name = strrchr(qry_name, '/') + 1;
    }

    sprintf(output_path, "%s%s%s-%s", BSD, BSD[strlen(BSD) - 1] == '/' ? "" : "/", geo_name, qry_name);
    sprintf(strrchr(output_path, '.'), ".svg"); 
    FILE* svg_file = fopen(output_path, "w");

    sprintf(strrchr(output_path, '.'), ".txt"); 
    FILE* txt_file = fopen(output_path, "w");

    fprintf(svg_file, "<svg xmlns=\"http://www.w3.org/2000/svg\">\n");
    
    char command[999];
    while (fscanf(qry_file, "%s", command) != EOF) {
        if (strcmp(command, "e") == 0) {
            
        } else if (strcmp(command, "mv") == 0) {
            
        } else if (strcmp(command, "lr") == 0) {
            
        } else if (strcmp(command, "d") == 0) {

        } else if (strcmp(command, "mc") == 0) {
            
        } else {
            printf("Comando %s nao reconhecido\n", command);
            fgets(command, 999, qry_file);
        }
    }

    sprintf(strrchr(output_path, '.'), ".dot"); 
    printSRB(shapes, output_path);

    return;
}