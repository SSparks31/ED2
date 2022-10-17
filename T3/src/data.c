#include "./data.h"

struct data {
    Shape shape;
    double value; // Energia para barcos, valor de incremento para outros
};

Data data_create(Shape shape, double value) {
    Data data = malloc(sizeof(struct data));
    if (!data) {
        return NULL;
    }

    data->shape = shape;
    data->value = value;
}

Shape data_get_shape(Data data) {
    if (!data) {
        return NULL;
    } 

    return data->shape;
}

void data_set_shape(Data data, Shape shape) {
    if (!data) {
        return;
    }

    data->shape = shape;
}

double data_get_value(Data data) {
    if (!data) {
        return -1;
    }

    return data->value;
}

void data_set_value(Data data, double value) {
    if (!data) { 
        return;
    }

    data->value = value;
}

void data_destroy(Data data) {
    if (!data) {
        return;
    }

    shape_destroy(&(data->shape));
    free(data);
}