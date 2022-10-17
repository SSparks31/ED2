#include "svg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CIRCLE_FORMAT "<circle cx=\"%.6lf\" cy=\"%.6lf\" r=\"%.6lf\" stroke=\"%s\" stroke-width=\"%.2lf\" fill=\"%s\" stroke-dasharray=\"%d\" />\n"
#define RECTANGLE_FORMAT "<rect x=\"%.6lf\" y=\"%.6lf\" width=\"%.6lf\" height=\"%.6lf\" stroke=\"%s\" stroke-width=\"%.2lf\" fill=\"%s\" stroke-dasharray=\"%d\" />\n"
#define LINE_FORMAT "<line x1=\"%.6lf\" y1=\"%.6lf\" x2=\"%.6lf\" y2=\"%.6lf\" stroke=\"%s\" stroke-width=\"%.2lf\" stroke-dasharray=\"%d\" />\n"
#define TEXT_FORMAT "<text x=\"%.6lf\" y=\"%.6lf\" stroke=\"%s\" stroke-width=\"%.2lf\" fill=\"%s\" font-size=\"%.0lf\" text-anchor=\"%s\">%s</text>\n"

struct shape {
    int id;

    char type;

    char* border_color;
    double border_width;

    char* fill_color;

    int is_dotted;

    char* text;
    char alignment;
    
    double coordinates[4];
};

int shape_get_id(Shape shape) {
    if (!shape) {
        return -1; 
    }

    return shape->id;
}

char shape_get_type(Shape shape) {
    if (!shape) {
        return '\0';
    }

    return shape->type;
}

void shape_set_border_color(Shape shape, char* new_color) {
    if (!shape || !new_color) {
        return;
    }

    shape->border_color = realloc(shape->border_color, strlen(new_color) + 1);
    strcpy(shape->border_color, new_color);
}

char* shape_get_border_color(Shape shape) {
    if (!shape) {
        return NULL;
    }

    return shape->border_color;
}

void shape_set_border_width(Shape shape, double width) {
    if (!shape || width < 0) {
        return;
    }

    shape->border_width = width;
}

double shape_get_border_width(Shape shape) {
    if (!shape) {
        return -1;
    }

    return shape->border_width;
}

void shape_set_fill_color(Shape shape, char* new_color) {
    if (!shape || !new_color) {
        return;
    }

    shape->fill_color = realloc(shape->fill_color, strlen(new_color) + 1);
    strcpy(shape->fill_color, new_color);
}

char* shape_get_fill_color(Shape shape) {
    if (!shape) {
        return NULL;
    }

    return shape->fill_color;
}

int shape_is_dotted(Shape shape) {
    if (!shape) {
        return -1;
    }

    return shape->is_dotted;
}

char* shape_get_text(Shape shape) {
    if (!shape) {
        return NULL;
    }

    return shape->text;
}

void shape_set_text(Shape shape, char* text) {
    if (!shape || !text) {
        return;
    }

    shape->text = realloc(shape->text, strlen(text) + 1);
    strcpy(shape->text, text);
}

char shape_get_alignment(Shape shape) {
    if (!shape) {
        return '\0';
    }

    return shape->alignment;
}

void shape_set_coordinate(Shape shape, int coordinate_position, double coordinate) {
    if (!shape || coordinate_position < 0 || coordinate_position > 3) {
        return;
    }

    shape->coordinates[coordinate_position] = coordinate;
}

double* shape_get_coordinates(Shape shape) {
    if (!shape) {
        return NULL;
    }

    return shape->coordinates;
}

Shape circle_create(int id, double x, double y, double r, char* border_color, char* fill_color, int is_dotted) {
    Shape new_circle = malloc(sizeof(struct shape));

    new_circle->id = id;

    new_circle->type = SVG_CIRCLE;

    new_circle->border_color = calloc(1, strlen(border_color) + 1);
    strcpy(new_circle->border_color, border_color);
    new_circle->border_width = 1;

    new_circle->fill_color = calloc(1, strlen(fill_color) + 1);
    strcpy(new_circle->fill_color, fill_color);
    
    new_circle->is_dotted = is_dotted;

    new_circle->text = NULL;
    new_circle->alignment = '\0';

    new_circle->coordinates[0] = x;
    new_circle->coordinates[1] = y;
    new_circle->coordinates[2] = r;

    return new_circle;
}

Shape rectangle_create(int id, double x, double y, double w, double h, char* border_color, char* fill_color, int is_dotted) {
    Shape new_rectangle = malloc(sizeof(struct shape));

    new_rectangle->id = id;

    new_rectangle->type = SVG_RECTANGLE;

    new_rectangle->border_color = calloc(1, strlen(border_color) + 1);
    strcpy(new_rectangle->border_color, border_color);
    new_rectangle->border_width = 1;

    new_rectangle->fill_color = calloc(1, strlen(fill_color) + 1);
    strcpy(new_rectangle->fill_color, fill_color);
    
    new_rectangle->is_dotted = is_dotted;

    new_rectangle->text = NULL;
    new_rectangle->alignment = '\0';

    new_rectangle->coordinates[0] = x;
    new_rectangle->coordinates[1] = y;
    new_rectangle->coordinates[2] = w;
    new_rectangle->coordinates[3] = h;

    return new_rectangle;
}

Shape line_create(int id, double x1, double y1, double x2, double y2, char* border_color, double border_width, int is_dotted) {
    Shape new_line = malloc(sizeof(struct shape));

    new_line->id = id;

    new_line->type = SVG_LINE;

    new_line->border_color = calloc(1, strlen(border_color) + 1);
    strcpy(new_line->border_color, border_color);
    new_line->border_width = border_width;

    new_line->fill_color = new_line->border_color;
    
    new_line->is_dotted = is_dotted;

    new_line->text = NULL;
    new_line->alignment = '\0';

    new_line->coordinates[0] = x1;
    new_line->coordinates[1] = y1;
    new_line->coordinates[2] = x2;
    new_line->coordinates[3] = y2;

    return new_line;
}

Shape text_create(int id, double x, double y, double size, char* border_color, char* fill_color, char alignment, char* text) {
    Shape new_text = malloc(sizeof(struct shape));

    new_text->id = id;

    new_text->type = SVG_TEXT;

    new_text->border_color = calloc(1, strlen(border_color) + 1);
    strcpy(new_text->border_color, border_color);
    new_text->border_width = .6;

    new_text->fill_color = calloc(1, strlen(fill_color) + 1);
    strcpy(new_text->fill_color, fill_color);
    
    new_text->is_dotted = 0;

    new_text->text = calloc(1, strlen(text) + 1);
    strcpy(new_text->text, text);
    new_text->alignment = alignment;

    new_text->coordinates[0] = x;
    new_text->coordinates[1] = y;
    new_text->coordinates[2] = size;

    return new_text;
}

Shape point_create(int id, double x, double y, char* border_color, char* fill_color, int is_dotted) {
    Shape new_point = malloc(sizeof(struct shape));

    new_point->id = id;

    new_point->type = SVG_POINT;

    new_point->border_color = calloc(1, strlen(border_color) + 1);
    strcpy(new_point->border_color, border_color);
    new_point->border_width = 1;

    new_point->fill_color = calloc(1, strlen(fill_color) + 1);
    strcpy(new_point->fill_color, fill_color);
    
    new_point->is_dotted = is_dotted;
    
    new_point->text = NULL;
    new_point->alignment = '\0';

    new_point->coordinates[0] = x;
    new_point->coordinates[1] = y;

    return new_point;
}

void circle_write_to_SVG(FILE* svg_file, Shape shape) {
    char* border_color = shape_get_border_color(shape);
    double border_width = shape_get_border_width(shape);
    
    char* fill_color = shape_get_fill_color(shape);
    
    int is_dotted = shape_is_dotted(shape);
 
    double x = shape_get_coordinates(shape)[0];
    double y = shape_get_coordinates(shape)[1];
    double r = shape_get_coordinates(shape)[2];
    
    fprintf(svg_file, CIRCLE_FORMAT, x, y, r, border_color, border_width, fill_color, is_dotted);
}

void rectangle_write_to_SVG(FILE* svg_file, Shape shape) {
    char* border_color = shape_get_border_color(shape);
    double border_width = shape_get_border_width(shape);
    
    char* fill_color = shape_get_fill_color(shape);
    
    int is_dotted = shape_is_dotted(shape) ? 1 : 0;
 
    double x = shape_get_coordinates(shape)[0];
    double y = shape_get_coordinates(shape)[1];
    double w = shape_get_coordinates(shape)[2];
    double h = shape_get_coordinates(shape)[3];
    
    fprintf(svg_file, RECTANGLE_FORMAT, x, y, w, h, border_color, border_width, fill_color, is_dotted);
}

void line_write_to_SVG(FILE* svg_file, Shape shape) {
    char* border_color = shape_get_border_color(shape);
    double border_width = shape_get_border_width(shape);
    
    int is_dotted = shape_is_dotted(shape) ? 1 : 0;
 
    double x1 = shape_get_coordinates(shape)[0];
    double y1 = shape_get_coordinates(shape)[1];
    double x2 = shape_get_coordinates(shape)[2];
    double y2 = shape_get_coordinates(shape)[3];
    
    fprintf(svg_file, LINE_FORMAT, x1, y1, x2, y2, border_color, border_width, is_dotted);
}

void text_write_to_SVG(FILE* svg_file, Shape shape) {
    char* border_color = shape_get_border_color(shape);
    double border_width = shape_get_border_width(shape);
    
    char* fill_color = shape_get_fill_color(shape);

    char* text = shape_get_text(shape);
    char alignment[9];
    switch (shape_get_alignment(shape)) {
    case SVG_TEXT_LEFT:
        strcpy(alignment, "start");
        break;
    
    case SVG_TEXT_CENTER:
        strcpy(alignment, "middle");
        break;

    case SVG_TEXT_RIGHT:
        strcpy(alignment, "end");
        break;

    default:
        printf("Ancora do texto desconhecida\n");
        return;
    }
    
    double x = shape_get_coordinates(shape)[0];
    double y = shape_get_coordinates(shape)[1];
    double size = shape_get_coordinates(shape)[2];

    fprintf(svg_file, TEXT_FORMAT, x, y, border_color, border_width, fill_color, size, alignment, text);
}

void point_write_to_SVG(FILE* svg_file, Shape shape) {
    char* border_color = shape_get_border_color(shape);
    double border_width = shape_get_border_width(shape);
    
    char* fill_color = shape_get_fill_color(shape);
    
    int is_dotted = shape_is_dotted(shape);
 
    double x = shape_get_coordinates(shape)[0];
    double y = shape_get_coordinates(shape)[1];
    
    fprintf(svg_file, CIRCLE_FORMAT, x, y, 1.0, border_color, border_width, fill_color, is_dotted);
}

void shape_write_to_SVG(FILE* svg_file, Shape shape) {
    if (!shape) {
        return;
    }

    switch (shape_get_type(shape)) {
    case SVG_CIRCLE:
        circle_write_to_SVG(svg_file, shape);
        break;
    
    case SVG_RECTANGLE:
        rectangle_write_to_SVG(svg_file, shape);
        break;
    
    case SVG_LINE:
        line_write_to_SVG(svg_file, shape);
        break;
    
    case SVG_TEXT:
        text_write_to_SVG(svg_file, shape);
        break;
    
    case SVG_POINT:
        circle_write_to_SVG(svg_file, shape);
        break;
    
    default:
        break;
    }
}

void shape_destroy(Shape* shape) {
    if ((*shape)->border_color != NULL) {
        free((*shape)->border_color);
    }
    if ((*shape)->fill_color != NULL) {
        free((*shape)->fill_color);
    }
    if ((*shape)->text != NULL) {
        free((*shape)->text);
    }

    free(*shape);
    *shape = NULL;
}

double point_distance(double x1, double y1, double x2, double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

int shape_inside_circle(Shape circle, Shape inside) {
    char inside_type = shape_get_type(inside);

    double* circle_coordinates = shape_get_coordinates(circle);
    double x_outside = circle_coordinates[0];
    double y_outside = circle_coordinates[1];
    double r_outside = circle_coordinates[2];

    double* inside_coordinates = shape_get_coordinates(inside);

    if (inside_type == SVG_CIRCLE) {

        double x_inside = inside_coordinates[0];
        double y_inside = inside_coordinates[1];
        double r_inside = inside_coordinates[2];

        return point_distance(x_inside, y_inside, x_outside, y_outside) + r_inside < r_outside + 0.00001;     

    } else if (inside_type == SVG_RECTANGLE) {
        double x_inside = inside_coordinates[0];
        double y_inside = inside_coordinates[1];
        double w_inside = inside_coordinates[2];
        double h_inside = inside_coordinates[3];
        
        return point_distance(x_inside, y_inside, x_outside, y_outside) < r_outside + 0.00001
        && point_distance(x_inside + w_inside, y_inside, x_outside, y_outside) < r_outside + 0.00001
        && point_distance(x_inside, y_inside + h_inside, x_outside, y_outside) < r_outside + 0.00001
        && point_distance(x_inside + w_inside, y_inside + h_inside, x_outside, y_outside) < r_outside + 0.00001;

    } else if (inside_type == SVG_LINE) {
        double x1_inside = inside_coordinates[0];
        double y1_inside = inside_coordinates[1];
        double x2_inside = inside_coordinates[2];
        double y2_inside = inside_coordinates[3];
        
        return point_distance(x1_inside, y1_inside, x_outside, y_outside) < r_outside + 0.00001
        && point_distance(x2_inside, y2_inside, x_outside, y_outside) < r_outside + 0.00001;

    } else if (inside_type == SVG_TEXT) {
        double x_inside = inside_coordinates[0];
        double y_inside = inside_coordinates[1];
        
        return point_distance(x_inside, y_inside, x_outside, y_outside) < r_outside + 0.00001;

    } else if (inside_type == SVG_POINT) {
        double x_inside = inside_coordinates[0];
        double y_inside = inside_coordinates[1];
        
        return point_distance(x_inside, y_inside, x_outside, y_outside) < r_outside + 0.00001;

    } 
    
    return -1;
}

int shape_inside_rectangle(Shape rectangle, Shape inside) {
    char inside_type = shape_get_type(inside);

    double* rectangle_coordinates = shape_get_coordinates(rectangle);
    double x_outside = rectangle_coordinates[0];
    double y_outside = rectangle_coordinates[1];
    double w_outside = rectangle_coordinates[2];
    double h_outside = rectangle_coordinates[3];

    double* inside_coordinates = shape_get_coordinates(inside);

    if (inside_type == SVG_CIRCLE) {
        double x_inside = inside_coordinates[0];
        double y_inside = inside_coordinates[1];
        double r_inside = inside_coordinates[2];

        return x_inside - r_inside >= x_outside
        && x_inside + r_inside <= x_outside + w_outside
        && y_inside - r_inside >= y_outside
        && y_inside + r_inside <= y_outside + h_outside;

    } else if (inside_type == SVG_RECTANGLE) {
        double x_inside = inside_coordinates[0];
        double y_inside = inside_coordinates[1];
        double w_inside = inside_coordinates[2];
        double h_inside = inside_coordinates[3];

        return x_inside >= x_outside
        && y_inside >= y_outside
        && x_inside + w_inside <= x_outside + w_outside
        && y_inside + h_inside <= y_outside + h_outside;

    } else if (inside_type == SVG_LINE) {
        double x1_inside = inside_coordinates[0];
        double y1_inside = inside_coordinates[1];
        double x2_inside = inside_coordinates[2];
        double y2_inside = inside_coordinates[3];

        return x1_inside >= x_outside
        && x1_inside <= x_outside + w_outside
        && y1_inside >= y_outside
        && y1_inside <= y_outside + h_outside
        && x2_inside >= x_outside
        && x2_inside <= x_outside + w_outside
        && y2_inside >= y_outside
        && y2_inside <= y_outside + h_outside;

    } else if (inside_type == SVG_TEXT) {
        double x_inside = inside_coordinates[0];
        double y_inside = inside_coordinates[1];

        return x_inside >= x_outside
        && x_inside <= x_outside + w_outside
        && y_inside >= y_outside
        && y_inside <= y_outside + h_outside;

    } else if (inside_type == SVG_POINT) {
        double x_inside = inside_coordinates[0];
        double y_inside = inside_coordinates[1];

        return x_inside >= x_outside
        && x_inside <= x_outside + w_outside
        && y_inside >= y_outside
        && y_inside <= y_outside + h_outside;
    } 
    
    return -1;
}

int compare(const void* a, const void* b) {
    double a_ = *(double*) a;
    double b_ = *(double*) b;

    if (a_ > b_) return 1;
    else if (a_ < b_) return -1;
    else return 0;
}

int shape_inside_line(Shape line, Shape inside) {
    char inside_type = shape_get_type(inside);

    double* line_coordinates = shape_get_coordinates(line);
    double x1_outside = line_coordinates[0];
    double y1_outside = line_coordinates[1];
    double x2_outside = line_coordinates[2];
    double y2_outside = line_coordinates[3];

    double* inside_coordinates = shape_get_coordinates(inside);

    if (inside_type == SVG_CIRCLE) {
        return 0;

    } else if (inside_type == SVG_RECTANGLE) {
        return 0;

    } else if (inside_type == SVG_LINE) {
        double x1_inside = inside_coordinates[0];
        double y1_inside = inside_coordinates[1];
        double x2_inside = inside_coordinates[2];
        double y2_inside = inside_coordinates[3];
        
        double a_outside = (x2_outside - x1_outside)/(y2_outside - y1_outside);
        double b_outside = y2_outside - a_outside * x1_outside;
        
        double a_inside = (x2_inside - x1_inside)/(y2_inside - y1_inside);
        double b_inside = y2_inside - a_inside * x1_inside;

        double x_sorted[4];
        x_sorted[0] = x1_inside;
        x_sorted[1] = x2_inside;
        x_sorted[2] = x1_outside;
        x_sorted[3] = x2_outside;
        qsort(x_sorted, 4, sizeof(int), &compare);

        return fabs(a_inside - a_outside) < 0.001
        && fabs(b_inside - b_outside) < 0.001
        && (x_sorted[0] == x1_outside || x_sorted[0] == x2_outside)
        && (x_sorted[3] == x1_outside || x_sorted[3] == x2_outside);

    } else if (inside_type == SVG_TEXT) {
        double x_inside = inside_coordinates[0];
        double y_inside = inside_coordinates[1];
        
        double a_outside = (x2_outside - x1_outside)/(y2_outside - y1_outside);
        double b_outside = y2_outside - a_outside * x1_outside;
        
        double x_sorted[3];
        x_sorted[0] = x_inside;
        x_sorted[1] = x1_outside;
        x_sorted[2] = x2_outside;
        qsort(x_sorted, 3, sizeof(int), &compare);

        return fabs(a_outside * x_inside + b_outside - y_inside) < 0.001
        && (x_sorted[0] == x1_outside || x_sorted[0] == x2_outside)
        && (x_sorted[2] == x1_outside || x_sorted[2] == x2_outside);

    } else if (inside_type == SVG_POINT) {
        double x_inside = inside_coordinates[0];
        double y_inside = inside_coordinates[1];
        
        double a_outside = (x2_outside - x1_outside)/(y2_outside - y1_outside);
        double b_outside = y2_outside - a_outside * x1_outside;
        
        double x_sorted[3];
        x_sorted[0] = x_inside;
        x_sorted[1] = x1_outside;
        x_sorted[2] = x2_outside;
        qsort(x_sorted, 3, sizeof(int), &compare);

        return fabs(a_outside * x_inside + b_outside - y_inside) < 0.001
        && (x_sorted[0] == x1_outside || x_sorted[0] == x2_outside)
        && (x_sorted[2] == x1_outside || x_sorted[2] == x2_outside);
    } 
    
    return -1;
}

int shape_inside_text(Shape text, Shape inside) {
    char inside_type = shape_get_type(inside);

    double* text_coordinates = shape_get_coordinates(text);
    double x_outside = text_coordinates[0];
    double y_outside = text_coordinates[1];

    double* inside_coordinates = shape_get_coordinates(inside);

    if (inside_type == SVG_CIRCLE) {
        return 0;

    } else if (inside_type == SVG_RECTANGLE) {
        return 0;

    } else if (inside_type == SVG_LINE) {
        return 0;

    } else if (inside_type == SVG_TEXT) {
        return x_outside == inside_coordinates[0]
        && y_outside == inside_coordinates[1];

    } else if (inside_type == SVG_POINT) {
        return x_outside == inside_coordinates[0]
        && y_outside == inside_coordinates[1];
    } 
    
    return -1;
}

int shape_inside_point(Shape point, Shape inside) {
    char inside_type = shape_get_type(inside);

    double* point_coordinates = shape_get_coordinates(point);
    double x_outside = point_coordinates[0];
    double y_outside = point_coordinates[1];

    double* inside_coordinates = shape_get_coordinates(inside);

    if (inside_type == SVG_CIRCLE) {
        return 0;
    
    } else if (inside_type == SVG_RECTANGLE) {
        return 0;
    
    } else if (inside_type == SVG_LINE) {
        return 0;
        
    } else if (inside_type == SVG_TEXT) {
        return x_outside == inside_coordinates[0]
        && y_outside == inside_coordinates[1];

    } else if (inside_type == SVG_POINT) {
        return x_outside == inside_coordinates[0]
        && y_outside == inside_coordinates[1];
    }  
    
    return -1;
}


int shape_inside(Shape outside, Shape inside) {
    if (!outside || !inside) {
        return -1;
    }

    char outside_type = shape_get_type(outside);

    switch (outside_type) {
    case SVG_CIRCLE:
        return shape_inside_circle(outside, inside);

    case SVG_RECTANGLE:
        return shape_inside_rectangle(outside, inside);

    case SVG_LINE:
        return shape_inside_line(outside, inside);

    case SVG_TEXT:
        return shape_inside_text(outside, inside);

    case SVG_POINT:
        return shape_inside_point(outside, inside);
    }    

    return -1;    
}