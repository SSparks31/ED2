#include "srbTree.h"

#include <stdlib.h>

typedef struct bbox {
    double x1;
    double x2;
    double y1;
    double y2;
} BBox;

struct srbTree {
    Node root;
    double epsilon;
};

struct node {
    SRBTree_elem elem;
    char color;
    double x;
    double y;

    Node parent;
    Node left;
    Node right;

    BBox bbox;
};

enum  {
    RED,
    BLACK
};

SRBTree createSRB(double epsilon) {
    SRBTree tree = malloc(sizeof(struct srbTree));
    if (!tree || epsilon < 0) {
        return NULL;
    }

    tree->root = NIL;
    tree->epsilon = epsilon; 

    return tree;
}

void fixInsertion(SRBTree t, Node new_node) {
    while (new_node->parent->color != BLACK) {
        break;
    }
}

Node insertSRB(SRBTree t, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, SRBTree_elem info) {
    if (!t) {
        return NULL;
    }

    Node new_node = malloc(sizeof(struct node));
    if (!new_node) {
        return NULL;
    }

    new_node->x = x;
    new_node->y = y;

    new_node->bbox.x1 = mbbX1;
    new_node->bbox.y1 = mbbY1;
    new_node->bbox.x2 = mbbX2;
    new_node->bbox.y2 = mbbY2;

    new_node->elem = info;

    new_node->left = NIL;
    new_node->right = NIL;

    if (t->root == NIL) {
        new_node->color = BLACK;
        new_node->parent = NULL;
        t->root = new_node;
    } else {
        new_node->color = RED;
        Node aux = t->root;

        for (;;) {
            if (fabs(x - aux->x) < t->epsilon && fabs(y - aux->y) < t->epsilon) { // Informacao ja existe
                free(new_node);
                return NULL;
            }

            if (x < aux->x || (fabs(x - aux->x) < t->epsilon && y < aux->y)) {
                if (aux->left == NIL) {
                    new_node->parent = aux;
                    aux->left = new_node;
                } else {
                    aux = aux->left;
                }
            } else {
                if (aux->right == NIL) {
                    new_node->parent = aux;
                    aux->right = new_node;
                } else {
                    aux = aux->right;
                }
            }
        }
        fixInsertion(t, new_node);
    }

    return new_node;
}

Node insertBbSRB(SRBTree t, double mbbX1, double mbbY1, double mbbX2, double mbbY2, SRBTree_elem info) {
    return insertSRB(t, mbbX1, mbbY1, mbbX1, mbbY1, mbbX2, mbbY2, info);
}

void getBbPartSRB(SRBTree t, double x, double y, double w, double h, List resultado) {

}

void getBbSRB(SRBTree t, double x, double y, double w, double h, List resultado) {

}

SRBTree_elem getInfoSRB(SRBTree t, Node n, double *xa, double *ya, double *mbbX1, double *mbbY1, double *mbbX2, double *mbbY2) {
    if (!t || !n) {
        return NULL;
    }

    *xa = n->x;
    *ya = n->x;
    *mbbX1 = n->bbox.x1;
    *mbbY1 = n->bbox.y1;
    *mbbX2 = n->bbox.x2;
    *mbbY2 = n->bbox.y2;

    return n->elem;
}

Node getNodeSRB(SRBTree t, double xa, double ya, double *mbbX1, double *mbbY1, double *mbbX2, double *mbbY2) {
    if (!t) {
        return NULL;
    }

    Node aux = t->root;

    while (aux) {
        if (fabs(aux->x - xa) < t->epsilon && fabs(aux->y - ya) < t->epsilon) {
            *mbbX1 = aux->bbox.x1;
            *mbbY1 = aux->bbox.y1;
            *mbbX2 = aux->bbox.x2;
            *mbbY2 = aux->bbox.y2;

            return aux;
        }
    }

    return NULL;
}

void updateInfoSRB(SRBTree t, Node n, SRBTree_elem i) {
    if (!t || !n) {
        return;
    }

    n->elem = i;
}

SRBTree_elem removeSRB(SRBTree t,double xa, double ya, double *mbbX1, double *mbbY1, double *mbbX2, double *mbbY2) {
    //aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
}

void printSRB(SRBTree t, char *nomeArq) {

}

void percursoLargura(SRBTree t, FvisitaNo fVisita, void *aux) {

}

void percursoSimetrico(SRBTree t, FvisitaNo fVisita, void *aux) {

}

void percursoProfundidade(SRBTree t, FvisitaNo fVisita, void *aux) {

}

void killSRBNodes(Node node) {
    if (!node) {
        return;
    }

    killSRBNodes(node->left);
    killSRBNodes(node->right);
    free(node);
}

void killSRB(SRBTree t) {
    if (!t) {
        return;
    }

    killSRBNodes(t->root);
    free(t);
}