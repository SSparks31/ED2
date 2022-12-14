#include "geo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

#include "srbTree.h"

#include "svg.h"


void geoImprimeFormas(SRBTree_elem i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* aux) {
    shape_write_to_SVG(aux,i);
    Shape bbox = rectangle_create(0, mbbX1 - 1, mbbY1 - 1, mbbX2 - mbbX1 + 2, mbbY2 - mbbY1 + 2, "red", "none", 1);
    shape_write_to_SVG(aux, bbox);
    shape_destroy(&bbox);
}

void c(SRBTree sea, FILE* geo_file, FILE* svg_file) {
    int id;

    double x;
    double y;
    double r;
    
    char border_color[99];
    char fill_color[99];

    fscanf(geo_file, "%d %lf %lf %lf %s %s\n", &id, &x, &y, &r, border_color, fill_color);

    Shape new_circle = circle_create(id, x, y, r, border_color, fill_color, 0);
    // Data fish = data_create(new_circle, 5);
    insertSRB(sea, x, y, x - r, y - r, x + r, y + r, new_circle);
}

void l(SRBTree sea, FILE* geo_file, FILE* svg_file) {
    int id;

    double x1;
    double y1;
    double x2;
    double y2;
    
    char border_color[99];

    fscanf(geo_file, "%d %lf %lf %lf %lf %s\n", &id, &x1, &y1, &x2, &y2, border_color);

    Shape new_line = line_create(id, x1, y1, x2, y2, border_color, 1, 0);

    if (x1 > x2) {
        double aux = x1;
        x1 = x2;
        x2 = aux;
    }

    if (y1 > y2) {
        double aux = y1;
        y1 = y2;
        y2 = aux;
    }

    // Data shrimp = data_create(new_line, 1);
    insertBbSRB(sea, x1, y1, x2, y2, new_line);
}

void r(SRBTree sea, FILE* geo_file, FILE* svg_file) {
    int id;

    double x;
    double y;
    double w;
    double h;
    
    char border_color[20];
    char fill_color[20];

    fscanf(geo_file, "%d %lf %lf %lf %lf %s %s\n", &id, &x, &y, &w, &h, border_color, fill_color);

    Shape new_rectangle = rectangle_create(id, x, y, w, h, border_color, fill_color, 0);
    // Data boat = data_create(new_rectangle, 0);
    insertBbSRB(sea, x, y, x + w, y + h, new_rectangle);
}

void t(SRBTree sea, FILE* geo_file, FILE* svg_file) {
    int id;

    double x;
    double y;

    char text[999];
    char alignment;

    char border_color[99];
    char fill_color[99];

    fscanf(geo_file, "%d %lf %lf %s %s %c ", &id, &x, &y, border_color, fill_color, &alignment);
    fgets(text, 998, geo_file);

    text[strcspn(text, "\r\n")] = '\0';

    Shape new_text = text_create(id, x, y, 8, border_color, fill_color, alignment, text);
    // Data item = data_create(new_text, 0);
    if (strcmp(text, ">-|-<") == 0) {
        shape_set_text(new_text, "&gt;-|-&lt;");
        // data_set_value(item, 20);
    } else if (strcmp(text, "$") == 0) {
        // data_set_value(item, 0.5);
    }

    insertSRB(sea, x, y, x - 2.3 * strlen(text), y - 7, x + 2.4 * strlen(text), y + 3, new_text);
}

void geo_parser(char* BED, char* BSD, char* geo_name, SRBTree sea) {
    if (!sea) {
        return;
    }

    char geo_path[PATH_MAX];
    sprintf(geo_path, "%s%s%s", BED, BED[strlen(BED) - 1] == '/' ? "" : "/", geo_name);
    FILE* geo_file = fopen(geo_path, "r");
    if (!geo_file) {
        return;
    }

    if (strrchr(geo_name, '/')) {
        geo_name = strrchr(geo_name, '/') + 1;
    }

    char output_path[PATH_MAX];
    sprintf(output_path, "%s%s%s", BSD, BSD[strlen(BSD) - 1] == '/' ? "" : "/", geo_name);

    sprintf(strrchr(output_path, '.'), ".svg");
    FILE* svg_file = fopen(output_path, "w");
    fprintf(svg_file, "<svg xmlns=\"http://www.w3.org/2000/svg\">\n");

    char command, buffer[999];

    while (fscanf(geo_file, "%c ", &command) != EOF) {
        switch (command) {
            case 'c':
                c(sea, geo_file, svg_file);
                break;

            case 'l':
                l(sea, geo_file, svg_file);
                break;

            case 'r':
                r(sea, geo_file, svg_file);
                break;

            case 't':
                t(sea, geo_file, svg_file);
                break;

            default:
                fgets(buffer, 999, geo_file);
                printf("Algo deu errado ao ler os comandos do arquivo geo\n");
                break;
        }   
    }

    percursoSimetrico(sea, geoImprimeFormas, svg_file);

    fprintf(svg_file, "</svg>");

    sprintf(strrchr(output_path, '.'), ".dot");
    printSRB(sea, output_path);

    fclose(geo_file);
    fclose(svg_file);
}