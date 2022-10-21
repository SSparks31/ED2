#include "qry.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <linux/limits.h>

#include "list.h"

#include "svg.h"

void qryImprimeFormas(SRBTree_elem i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* aux) {
    shape_write_to_SVG(aux, i);
    Shape bbox = rectangle_create(0, mbbX1 - 1, mbbY1 - 1, mbbX2 - mbbX1 + 2, mbbY2 - mbbY1 + 2, "red", "none", 1);
    shape_write_to_SVG(aux, bbox);
    shape_destroy(&bbox);
}

struct eInfo {
    double energy;
    FILE* txt_file;
};

void energize(SRBTree_elem i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void *aux) {
    Shape shape = i;
    struct eInfo* info = aux;
    if (shape_get_type(shape) == SVG_RECTANGLE) {
        // data_set_value(i, info->energy);
        fprintf(info->txt_file, "\tNau %d energizada em %.2lf\n", shape_get_id(shape), info->energy);
    }
}

void e(FILE* qry_file, FILE* txt_file, SRBTree sea) {
    fprintf(txt_file, "=== E ===\n");
    
    double energy;
    fscanf(qry_file, " %lf\n", &energy);
    // struct eInfo info;
    // info.energy = energy;
    // info.txt_file = txt_file;
    // percursoLargura(sea, energize, &info);
}

struct mvInfo {
    SRBTree t;
    FILE* txt_file;
    int id;
    double dx;
    double dy;
    int done;
};

void mv_(SRBTree_elem i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void *aux) {
    struct mvInfo* info = aux;
    if (info->done) {
        return;
    }

    // Shape shape = data_get_shape(i);

    // if (shape_get_id(shape) == info->id) {
    //     info->done = 1;
    //     removeSRB(info->t, x, y,&mbbX1, &mbbY1,&mbbX2, &mbbY2);
        
    //     switch (shape_get_type(shape)) {
    //     case SVG_RECTANGLE :
    //         fprintf(info->txt_file, "\tMovendo nau de ID %d de (%.2lf %.2lf) para (%.2lf %.2lf)\n", info->id, x, y, x + info->dx, y + info->dy);
    //         shape_set_coordinate(shape, 0, x + info->dx);
    //         shape_set_coordinate(shape, 1, y + info->dy);
    //         insertSRB(info->t, x + info->dx, y + info->dy, mbbX1 + info->dx, mbbY1 + info->dy, mbbX2 + info->dx, mbbY2 + info->dy, i);
    //         break;
    //     case SVG_CIRCLE:
    //         fprintf(info->txt_file, "\tMovendo peixe de ID %d de (%.2lf %.2lf) para (%.2lf %.2lf)\n", info->id, x, y, x + info->dx, y + info->dy);
    //         shape_set_coordinate(shape, 0, x + info->dx);
    //         shape_set_coordinate(shape, 1, y + info->dy);
    //         insertSRB(info->t, x + info->dx, y + info->dy, mbbX1 + info->dx, mbbY1 + info->dy, mbbX2 + info->dx, mbbY2 + info->dy, i);
    //         break;
    //     case SVG_LINE:
    //         fprintf(info->txt_file, "\tMovendo camarao de ID %d de (%.2lf %.2lf) para (%.2lf %.2lf)\n", info->id, mbbX1, mbbY1, mbbX1 + info->dx, mbbY1 + info->dy);
    //         shape_set_coordinate(shape, 0, mbbX1 + info->dx);
    //         shape_set_coordinate(shape, 1, mbbY1 + info->dy);
    //         shape_set_coordinate(shape, 2, mbbX2 + info->dx);
    //         shape_set_coordinate(shape, 3, mbbY2 + info->dy);
    //         insertSRB(info->t, x + info->dx, y + info->dy, mbbX1 + info->dx, mbbY1 + info->dy, mbbX2 + info->dx, mbbY2 + info->dy, i);
    //         break;
    //     case SVG_TEXT:
    //         if (strcmp(shape_get_text(shape), ">-|-<") == 0) {
    //             fprintf(info->txt_file, "\tMovendo lagosta ");
    //         } else {
    //             fprintf(info->txt_file, "\tMovendo lixo ");
    //         }

    //         fprintf(info->txt_file, "de ID %d de (%.2lf %.2lf) para (%.2lf %.2lf)\n", info->id, x, y, x + info->dx, y + info->dy);
    //         shape_set_coordinate(shape, 0, mbbX1 + info->dx);
    //         shape_set_coordinate(shape, 1, mbbY1 + info->dy);
    //         insertSRB(info->t, x + info->dx, y + info->dy, mbbX1 + info->dx, mbbY1 + info->dy, mbbX2 + info->dx, mbbY2 + info->dy, i);

    //         break;
    //     }
    // }
}

void mv(FILE* qry_file, FILE* txt_file, SRBTree sea) {
    fprintf(txt_file, "=== MV ===\n");
    
    int id;
    double dx, dy;
    fscanf(qry_file, " %d %lf %lf\n", &id, &dx, &dy);

    // struct mvInfo info;
    // info.t = sea;
    // info.txt_file = txt_file;
    // info.id = id;
    // info.dx = dx;
    // info.dy = dy;
    // info.done = 0;

    // percursoSimetrico(sea, mv_, &info);
}

void lr(FILE* qry_file, FILE* txt_file, SRBTree sea) {
    fprintf(txt_file, "=== MV ===\n");

    int id;
    char lado[3];
    double d;
    double w;
    double h;

    fscanf(qry_file, " %d %s %lf %lf %lf\n", &id, lado, &d, &w, &h);
}

void d(FILE* qry_file, FILE* txt_file, SRBTree sea) {
    fprintf(txt_file, "=== D ===\n");
    int id;
    char lado[3];
    double d;

    fscanf(qry_file, " %d %s %lf\n", &id, lado, &d);
}

void mc(FILE* qry_file, FILE* txt_file, SRBTree sea) {
    fprintf(txt_file, "=== MC ===\n");
    double dx, dy, x, y, w, h;
    fscanf(qry_file, " %lf %lf %lf %lf %lf %lf\n", &dx, &dy, &x, &y, &w, &h);
}

void qry_parser(char* BED, char* BSD, char* geo_name, char* qry_name, SRBTree sea) {    
    char qry_path[PATH_MAX];
    sprintf(qry_path, "%s%s%s", BED, BED[strlen(BED) - 1] == '/' ? "" : "/", qry_name);
    FILE* qry_file = fopen(qry_path, "r");
    if (!qry_file) {
        return;
    }

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

    if (!svg_file || !qry_file) {
        return;
    }

    fprintf(svg_file, "<svg xmlns=\"http://www.w3.org/2000/svg\">\n");
    
    char command[999];
    while (fscanf(qry_file, "%s", command) != EOF) {
        if (strcmp(command, "e") == 0) {
            e(qry_file, txt_file, sea);
        } else if (strcmp(command, "mv") == 0) {
            mv(qry_file, txt_file, sea);
        } else if (strcmp(command, "lr") == 0) {
            lr(qry_file, txt_file, sea);
        } else if (strcmp(command, "d") == 0) {
            d(qry_file, txt_file, sea);
        } else if (strcmp(command, "mc") == 0) {
            mc(qry_file, txt_file, sea);
        } else {
            printf("Comando %s nao reconhecido\n", command);
            fgets(command, 999, qry_file);
        }
    }

    percursoSimetrico(sea, qryImprimeFormas, svg_file);

    fprintf(svg_file, "</svg>");

    sprintf(strrchr(output_path, '.'), ".dot"); 
    printSRB(sea, output_path);

    fclose(txt_file);
    fclose(svg_file);

    return;
}