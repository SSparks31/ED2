#include "qry.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "list.h"

#include "svg.h"
#include "data.h"

void qry_parser(char* BED, char* BSD, char* geo_name, char* qry_name, SRBTree shapes) {    
    if (!qry_name || strlen(qry_name) == 0) {
        return;
    }

    char* qry_path = calloc(1, strlen(BED) + 1 + strlen(qry_name) + 1);
    if (BED[strlen(BED)-1] != '/') {
        sprintf(qry_path, "%s/%s", BED, qry_name);
    } else {
        sprintf(qry_path, "%s%s", BED, qry_name);
    }

    FILE* qry_file = fopen(qry_path, "r");
    if (!qry_file) {
        return;
    }

    if (strrchr(geo_name, '/')) {
        geo_name = strrchr(geo_name, '/') + 1;
    }
    qry_name = strrchr(qry_path, '/') + 1;
    *strrchr(geo_name, '.') = '\0';
    *strrchr(qry_name, '.') = '\0';

    char* svg_path = calloc(1, strlen(BSD) + 1 + strlen(geo_name) + 1 + strlen(qry_name) + 4 + 1);

    if (BSD[strlen(BSD)-1] != '/') {
        sprintf(svg_path, "%s/%s-%s.svg", BSD, geo_name, qry_name);
    } else {
        sprintf(svg_path, "%s%s-%s.svg", BSD, geo_name, qry_name);
    }
    
    FILE* svg_file = fopen(svg_path, "w");
    free(svg_path);

    fprintf(svg_file, "<svg xmlns=\"http://www.w3.org/2000/svg\">\n");
    
    char command[999];
    while (fscanf(qry_file, "%s", command) != EOF) {
        if (strcmp(command, "e") == 0) {
            
        } else if (strcmp(command, "mv") == 0) {
            
        } else if (strcmp(command, "lr") == 0) {
            
        } else if (strcmp(command, "d") == 0) {

        } else if (strcmp(command, "mc") == 0) {
            
        } else {
            printf("Comando nao reconhecido\n");
            fgets(command, 999, qry_file);
        }
    }
    
    fprintf(svg_file, "</svg>");

    fclose(qry_file);
    fclose(svg_file);

    free(qry_path);

    return;
}