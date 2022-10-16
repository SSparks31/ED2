#ifndef LIB_SVG_H
#define LIB_SVG_H

/* Modulo de manipulação de figuras SVG */

#include <stdio.h>

typedef struct shape* Shape;

enum {
    SVG_TEXT_LEFT = 'i',
    SVG_TEXT_CENTER = 'm',
    SVG_TEXT_RIGHT = 'f',
};

enum {
    SVG_CIRCLE,
    SVG_RECTANGLE,
    SVG_LINE,
    SVG_TEXT,
    SVG_POINT
};


/* Retorna o ID armazenado em `shape`, ou -1 caso `shape` seja invalido */
int shape_get_id(Shape shape);

/* Retorna o tipo da forma armazenada em `shape` */
/* Caso `shape` seja invalido, retorna o caracter nulo '\0' */
char shape_get_type(Shape shape);

/* Altera a cor da borda de `shape` */
void shape_set_border_color(Shape shape, char* new_color);

/* Retorna a cor da borda de `shape` */
char* shape_get_border_color(Shape shape);

/* Altera a largura da borda de `shape` */
void shape_set_border_width(Shape shape, double width);

/* Retorna a largura da borda de `shape` */
double shape_get_border_width(Shape shape);

/* Altera a cor de preenchimento de `shape` */
void shape_set_fill_color(Shape shape, char* new_color);

/* Retorna a cor de preenchimento de `shape` */
char* shape_get_fill_color(Shape shape);

/* Retorna 1 caso a forma de `shape` seja pontilhada, 0 caso contrário */
int shape_is_dotted(Shape shape);

/* Retorna o texto armazenado em `shape` */
char* shape_get_text(Shape shape);

/* Retorna a posição do texto de `shape` relativa à sua ancora */
char shape_get_alignment(Shape shape);

/* Altera uma das quatro coordenadas de `shape` de acordo com o tipo da forma */
/* Retangulo: x y w h */
/* Circulo: x y r */
/* Reta: x1 y1 x2 y2 */
/* Texto: x y tamanho*/ 
void shape_set_coordinate(Shape shape, int coordinate_position, double coordinate);

/* Retorna o vetor de coordenadas de `shape` */
double* shape_get_coordinates(Shape shape);

/* Cria e retorna um circulo com as dadas propriedades */
Shape circle_create(int id, double x, double y, double r, char* border_color, char* fill_color, int is_dotted);

/* Cria e retorna um retângulo com as dadas propriedades */
Shape rectangle_create(int id, double x, double y, double w, double h, char* border_color, char* fill_color, int is_dotted);

/* Cria e retorna uma reta com as dadas propriedades */
Shape line_create(int id, double x1, double y1, double x2, double y2, char* border_color, double border_width, int is_dotted);

/* Cria e retorna um texto com as dadas propriedades */
Shape text_create(int id, double x, double y, double size, char* border_color, char* fill_color, char alignment, char* text);

/* Cria e retorna um ponto com as dadas propriedades */
Shape point_create(int id, double x, double y, char* border_color, char* fill_color, int is_dotted);

/* Adiciona a forma de `shape` ao arquivo `svg_file` */
void shape_write_to_SVG(FILE* svg_file, Shape shape);

/* Destroi a estrutura apontada por `shape` */
void shape_destroy(Shape* shape);

/* Verifica se a forma de `inside` esta dentro da forma de `outside` */
/* Caso verdadeiro, retorna 1; 0, caso contrario */
/* Caso `outside` ou `inside` sejam invalidas, retorna -1 */
int shape_inside(Shape outside, Shape inside);


#endif