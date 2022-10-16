#include "geo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "svg.h"

void c(SRBTree shapes, FILE* geo_file, FILE* svg_file) {
    int id;

    double x;
    double y;
    double r;
    
    char border_color[99];
    char fill_color[99];

    fscanf(geo_file, "%d %lf %lf %lf %s %s\n", &id, &x, &y, &r, border_color, fill_color);

    Shape new_circle = circle_create(id, x, y, r, border_color, fill_color, 0);
    Boat new_boat = boat_create(new_circle, 60);
    xyytree_insert(shapes, x, y, new_boat);
    shape_write_to_SVG(svg_file, new_circle);
}

void l(SRBTree shapes, FILE* geo_file, FILE* svg_file) {
    int id;

    double x1;
    double y1;
    double x2;
    double y2;
    
    char border_color[99];

    fscanf(geo_file, "%d %lf %lf %lf %lf %s\n", &id, &x1, &y1, &x2, &y2, border_color);

    Shape new_line = line_create(id, x1, y1, x2, y2, border_color, 1, 0);
    Boat new_boat = boat_create(new_line, 50);
    xyytree_insert(shapes, x1, y1, new_boat);
    shape_write_to_SVG(svg_file, new_line);
}

void r(SRBTree shapes, FILE* geo_file, FILE* svg_file) {
    int id;

    double x;
    double y;
    double w;
    double h;
    
    char border_color[20];
    char fill_color[20];

    fscanf(geo_file, "%d %lf %lf %lf %lf %s %s\n", &id, &x, &y, &w, &h, border_color, fill_color);

    Shape new_rectangle = rectangle_create(id, x, y, w, h, border_color, fill_color, 0);
    Boat new_boat = boat_create(new_rectangle, 60);
    xyytree_insert(shapes, x, y, new_boat);
    shape_write_to_SVG(svg_file, new_rectangle);
}

void t(SRBTree shapes, FILE* geo_file, FILE* svg_file) {
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

    Shape new_text = text_create(id, x, y, 20, border_color, fill_color, alignment, text);
    Boat new_boat = boat_create(new_text, 5);
    xyytree_insert(shapes, x, y, new_boat);
    shape_write_to_SVG(svg_file, new_text);
}

void geo_parser(char* BED, char* BSD, char* geo_name, SRBTree shapes) {
    if (!BED || strlen(BED) == 0 || !BSD || strlen(BSD) == 0 || !geo_name || strlen(geo_name) == 0 || !shapes) {
        return;
    }

    char* geo_path = calloc(1, strlen(BED) + 1 + strlen(geo_name) + 1);
    if (BED[strlen(BED)-1] != '/') {
        sprintf(geo_path, "%s/%s", BED, geo_name);
    } else {
        sprintf(geo_path, "%s%s", BED, geo_name);
    }

    FILE* geo_file = fopen(geo_path, "r");


    geo_name = strrchr(geo_path, '/') + 1;
    char* svg_path = calloc(1, strlen(BSD) + 1 + strlen(geo_name) + 1);
    if (BSD[strlen(BSD)-1] != '/') {
        sprintf(svg_path, "%s/%s", BSD, geo_name);
    } else {
        sprintf(svg_path, "%s%s", BSD, geo_name);
    }    
    char* extension = strrchr(svg_path, '.');
    sprintf(extension + 1, "svg");

    FILE* svg_file = fopen(svg_path, "w");
    free(svg_path);
    if (!svg_file) {
        fclose(geo_file);
        return;
    }

    fprintf(svg_file, "<svg xmlns=\"http://www.w3.org/2000/svg\">\n");

    char command, buffer[999];

    while (fscanf(geo_file, "%c ", &command) != EOF) {
        switch (command) {
            case 'c':
                c(shapes, geo_file, svg_file);
                break;

            case 'l':
                l(shapes, geo_file, svg_file);
                break;

            case 'r':
                r(shapes, geo_file, svg_file);
                break;

            case 't':
                t(shapes, geo_file, svg_file);
                break;

            default:
                fgets(buffer, 999, geo_file);
                printf("Algo deu errado ao ler os comandos do arquivo geo\n");
                break;
        }   
    }

    fprintf(svg_file, "</svg>");

    free(geo_path);

    fclose(geo_file);
    fclose(svg_file);
}