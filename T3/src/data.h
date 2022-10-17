#ifndef LIB_BOAT_H
#define LIB_BOAT_H

#include "svg.h"


typedef struct data* Data;

Data data_create(Shape shape, double value);

Shape data_get_shape(Data data);

void data_set_shape(Data data, Shape shape);

double data_get_value(Data data);

void data_set_value(Data data, double value);

void data_destroy(Data data);

#endif