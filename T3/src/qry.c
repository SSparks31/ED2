#include "qry.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "list.h"

#include "svg.h"
#include "boat.h"

void na(FILE* qry_file, FILE* txt_file, double* attack) {
    fscanf(qry_file, " %lf\n", attack);

    fprintf(txt_file, "===\nNA\n\tAgressividade ajustada para %.2lf\n", *attack);
}

int tp_inside(XyyTree_elem boat, double x, double y) {
    Shape shape = boat_get_shape(boat);
    Shape point = point_create(0, x, y, "", "", 0);
    int is_inside = shape_inside(shape, point);
    shape_destroy(&point);

    return is_inside;
}

double tp(FILE* qry_file, FILE* txt_file, XyyTree boats, List new_shapes) {
    fprintf(txt_file, "===\nTP\n");

    double x, y;
    fscanf(qry_file, " %lf %lf\n", &x, &y);

    List hits = xyytree_get_elems_containing_point(boats, x, y, tp_inside);
    List_pos aux = list_get_first(hits);
    double score = 0;

    Shape hit_text;
    if (list_get_size(hits) == 0) {
        hit_text = text_create(0, x, y + 4, 8, "gray", "gray", 'm', "*");
    } else if (list_get_size(hits) == 1) {
        hit_text = text_create(0, x, y + 4, 8, "red", "red", 'm', "*");
    } else if (list_get_size(hits) == 2) {
        char text[10];
        sprintf(text, "*%d", list_get_size(hits));
        hit_text = text_create(0, x, y + 4, 8, "red", "red", 'm', text);
    }

    list_append(new_shapes, hit_text);

    while ((aux = list_get_first(hits))) {
        Boat boat = list_get_elem(hits, aux);
        Shape shape = boat_get_shape(boat);
        char type = shape_get_type(shape);
        double* coordinates = shape_get_coordinates(shape);

        x = coordinates[0];
        y = coordinates[1];

        xyytree_remove_node(boats, x, y, 0);

        if (type == SVG_RECTANGLE) {
            fprintf(txt_file, "\tRetangulo ID %d - Ancora (%.2lf, %.2lf) - Largura %.2lf, Altura %.2lf - Protecao %.6lf\n", shape_get_id(shape), x, y, coordinates[2], coordinates[3], boat_get_shields(boat));
            score += 450/(coordinates[2] * coordinates[3]);
        } else if (type == SVG_CIRCLE) {
            fprintf(txt_file, "\tCirculo ID %d - Ancora (%.2lf, %.2lf) - Raio %.2lf - Protecao %.6lf\n", shape_get_id(shape), x, y, coordinates[2], boat_get_shields(boat));
            score += 375/(coordinates[2] * coordinates[2] * 3.14);
        } else if (type == SVG_LINE) {
            fprintf(txt_file, "\tReta ID %d - Ancora 1 (%.2lf, %.2lf) - Ancora 2 (%.2lf, %.2lf) - Protecao %.6lf\n", shape_get_id(shape), x, y, coordinates[2], coordinates[3], boat_get_shields(boat));
            score += 150;
        } else if (type == SVG_TEXT) {
            fprintf(txt_file, "\tTexto ID %d - Ancora (%.2lf, %.2lf) - Conteudo `%s` - Protecao %.6lf\n", shape_get_id(shape), x, y, shape_get_text(shape), boat_get_shields(boat));
            score += 500;
        }

        boat_destroy(&boat);

        list_remove(hits, aux);
    }

    if (score == 0) {
        fprintf(txt_file, "\tAGUA\n");
    }

    list_destroy(&hits);

    return score;
}

void tr(FILE* qry_file, FILE* txt_file, XyyTree boats, List new_shapes) {
    fprintf(txt_file, "===\nTR\n");
    
    double x, y, dx, dy;
    int id;
    fscanf(qry_file, " %lf %lf %lf %lf %d\n", &x, &y, &dx, &dy, &id);
    
    Shape text = text_create(0, x, y + 2, 8, "red", "red", 'm', "@");
    list_append(new_shapes, text);

    List hits = xyytree_get_elems_containing_point(boats, x, y, tp_inside);
    List_pos aux = list_get_first(hits);
    
    while ((aux = list_get_first(hits))) {
        Boat boat = list_get_elem(hits, aux);
        Shape shape = boat_get_shape(boat);
        
        char type = shape_get_type(shape);
        double* coordinates = shape_get_coordinates(shape);
        char* border_color = shape_get_border_color(shape);
        char* fill_color = shape_get_fill_color(shape);
        
        Shape new_shape;

        switch (type) {
        case SVG_CIRCLE:
            fprintf(txt_file, "\tCirculo ID %d - Ancora (%.2lf, %.2lf) - Raio %.2lf - Protecao %.6lf\n", shape_get_id(shape), coordinates[0], coordinates[1], coordinates[2], boat_get_shields(boat));
            fprintf(txt_file, "\tClone ID %d - Ancora (%.2lf, %.2lf) - Raio %.2lf - Protecao %.6lf\n\n", id, coordinates[0] + dx, coordinates[1] + dy, coordinates[2], boat_get_shields(boat));
            new_shape = circle_create(id, coordinates[0] + dx, coordinates[1] + dy, coordinates[2], fill_color, border_color, 0);
            break;

        case SVG_RECTANGLE:
            fprintf(txt_file, "\tRetangulo ID %d - Ancora (%.2lf, %.2lf) - Largura %.2lf, Altura %.2lf - Protecao %.6lf\n", shape_get_id(shape), coordinates[0], coordinates[1], coordinates[2], coordinates[3], boat_get_shields(boat));
            fprintf(txt_file, "\tClone ID %d - Ancora (%.2lf, %.2lf) - Largura %.2lf, Altura %.2lf - Protecao %.6lf\n\n", id, coordinates[0] + dx, coordinates[1] + dy, coordinates[2], coordinates[3], boat_get_shields(boat));
            new_shape = rectangle_create(id, coordinates[0] + dx, coordinates[1] + dy, coordinates[2], coordinates[3], fill_color, border_color, 0);
            break;

        case SVG_LINE:
            fprintf(txt_file, "\tReta ID %d - Ancora 1 (%.2lf, %.2lf) - Ancora 2 (%.2lf, %.2lf) - Protecao %.6lf\n", shape_get_id(shape), coordinates[0], coordinates[1], coordinates[2], coordinates[3], boat_get_shields(boat));
            fprintf(txt_file, "\tClone ID %d - Ancora 1 (%.2lf, %.2lf) - Ancora 2 (%.2lf, %.2lf) - Protecao %.6lf\n\n", id, coordinates[0] + dx, coordinates[1] + dy, coordinates[2] + dx, coordinates[3] + dy, boat_get_shields(boat));
            new_shape = line_create(id, coordinates[0] + dx, coordinates[1] + dy, coordinates[2] + dx, coordinates[3] + dy, border_color, shape_get_border_width(shape), 0);
            break;

        case SVG_TEXT:
            fprintf(txt_file, "\tTexto ID %d - Ancora (%.2lf, %.2lf) - Conteudo `%s` - Protecao %.6lf\n", shape_get_id(shape), coordinates[0], coordinates[1], shape_get_text(shape), boat_get_shields(boat));
            fprintf(txt_file, "\tClone ID %d - Ancora (%.2lf, %.2lf) - Conteudo `%s` - Protecao %.6lf\n\n", id, coordinates[0] + dx, coordinates[1] + dy, shape_get_text(shape), boat_get_shields(boat));
            new_shape = text_create(id, coordinates[0] + dx, coordinates[1] + dy, coordinates[2], fill_color, border_color, shape_get_alignment(shape), shape_get_text(shape));
            break;
        }

        Boat new_boat = boat_create(new_shape, boat_get_shields(boat));
        xyytree_insert(boats, coordinates[0] + dx, coordinates[1] + dy, new_boat);

        id++;
        list_remove(hits, aux);
    }

    list_destroy(&hits);
}

int be_inside(XyyTree_elem boat, double x1, double y1, double x2, double y2) {
    Shape shape = boat_get_shape(boat);
    Shape box = rectangle_create(0, x1, y1, x2, y2, "", "", 0);
    int is_inside = shape_inside(box, shape);
    shape_destroy(&box);

    return is_inside;
}

double be(FILE* qry_file, FILE* txt_file, XyyTree boats, double attack, List new_shapes) {
    fprintf(txt_file, "===\nBE\n");
    double x, y, w, h;
    fscanf(qry_file, " %lf %lf %lf %lf\n", &x, &y, &w, &h);

    List hits = xyytree_get_elems_in_box(boats, x, y, x + w, y + h, be_inside);
    List_pos aux;

    Shape box = rectangle_create(0, x, y, w, h, "red", "none", 1);
    list_append(new_shapes, box);

    double score = 0;

    while ((aux = list_get_first(hits))) {
        Boat boat = list_get_elem(hits, aux);
        double shields = boat_get_shields(boat);
        
        Shape shape = boat_get_shape(boat);
        double area;
        double adder;
        double* coordinates = shape_get_coordinates(shape);

        Shape text = text_create(0, coordinates[0], coordinates[1] + .6, 20, "red", "red", 'm', ".");
        list_append(new_shapes, text);

        switch (shape_get_type(shape)) {
        case SVG_RECTANGLE:
            fprintf(txt_file, "\tRetangulo ID %d - Ancora (%.2lf, %.2lf) - Largura %.2lf, Altura %.2lf", shape_get_id(shape), coordinates[0], coordinates[1], coordinates[2], coordinates[3]);
            area = coordinates[2] * coordinates[3];
            adder = 90;
            break;
        
        case SVG_CIRCLE:
            fprintf(txt_file, "\tCirculo ID %d - Ancora (%.2lf, %.2lf) - Raio %.2lf", shape_get_id(shape), coordinates[0], coordinates[1], coordinates[2]);
            area = coordinates[2] * coordinates[2] * 3.14;
            adder = 75;
            break;

        case SVG_LINE:
            fprintf(txt_file, "\tReta ID %d - Ancora 1 (%.2lf, %.2lf) - Ancora 2 (%.2lf, %.2lf)", shape_get_id(shape), coordinates[0], coordinates[1], coordinates[2], coordinates[3]);
            area = 0.1 * sqrt((coordinates[2] - coordinates[0]) * (coordinates[2] - coordinates[0]) + (coordinates[3] - coordinates[1]) * (coordinates[3] - coordinates[1]));
            adder = 50;
            break;

        case SVG_TEXT:
            fprintf(txt_file, "\tTexto ID %d - Ancora (%.2lf, %.2lf) - Conteudo `%s`", shape_get_id(shape), coordinates[0], coordinates[1], shape_get_text(shape));
            area = 0.1;
            adder = 30;
            break;

        }

        shields -= (attack * area)/(w*h);
        fprintf(txt_file, " - Protecao %.6lf", shields);
        if (shields <= 0) {
            fprintf(txt_file, " - REMOVIDA");
            shields = 0;
            xyytree_remove_node(boats, coordinates[0], coordinates[1], 0);
            score += adder;
        }
        boat_set_shields(boat, shields);

        list_remove(hits, aux);

        fprintf(txt_file, "\n");
    }

    list_destroy(&hits);

    return score;
}

void breadth_list_filler(XyyTree_elem elem, double x, double y, void* reg) {
    list_append(reg, elem);
}

void qry_parser(char* BED, char* BSD, char* geo_name, char* qry_name, XyyTree boats) {    
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
    char* txt_path = calloc(1, strlen(BSD) + 1 + strlen(geo_name) + 1 + strlen(qry_name) + 4 + 1);

    if (BSD[strlen(BSD)-1] != '/') {
        sprintf(svg_path, "%s/%s-%s.svg", BSD, geo_name, qry_name);
    } else {
        sprintf(svg_path, "%s%s-%s.svg", BSD, geo_name, qry_name);
    }

    strncpy(txt_path, svg_path, strlen(svg_path) - 3);
    strcat(txt_path, "txt");
    
    FILE* svg_file = fopen(svg_path, "w");
    FILE* txt_file = fopen(txt_path, "w");
    free(svg_path);    
    free(txt_path);

    fprintf(svg_file, "<svg xmlns=\"http://www.w3.org/2000/svg\">\n");
    
    double score = 0;
    double attack = 0;

    List new_shapes = list_create();

    char command[999];
    while (fscanf(qry_file, "%s", command) != EOF) {
        if (strcmp(command, "na") == 0) {
            na(qry_file, txt_file, &attack);
        } else if (strcmp(command, "tp") == 0) {
            score += tp(qry_file, txt_file, boats, new_shapes);
        } else if (strcmp(command, "tr") == 0) {
            tr(qry_file, txt_file, boats, new_shapes);
        } else if (strcmp(command, "be") == 0) {
            score += be(qry_file, txt_file, boats, attack, new_shapes);
        } else {
            printf("Comando nao reconhecido\n");
            fgets(command, 999, qry_file);
        }
    }

    List svg_boats = list_create();
    xyytree_breadth_search(boats, breadth_list_filler, svg_boats);

    List_pos aux;

    while ((aux = list_get_first(svg_boats))) {
        Boat boat = list_get_elem(svg_boats, aux);
        Shape shape = boat_get_shape(boat);

        shape_write_to_SVG(svg_file, shape);

        boat_destroy(&boat);
        list_remove(svg_boats, aux);
    }

    while ((aux = list_get_first(new_shapes))) {
        Shape shape = list_get_elem(new_shapes, aux);

        shape_write_to_SVG(svg_file, shape);

        shape_destroy(&shape);
        list_remove(new_shapes, aux);
    }

    list_destroy(&svg_boats);
    list_destroy(&new_shapes);

    fprintf(txt_file, "===\nPONTUACAO FINAL: %.6lf\n", score);
    
    fprintf(svg_file, "</svg>");
    fprintf(txt_file, "===");

    fclose(qry_file);
    fclose(svg_file);
    fclose(txt_file);

    free(qry_path);

    return;
}