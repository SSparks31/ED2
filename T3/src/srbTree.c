#include "srbTree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "svg.h"
#include "queue.h"

typedef struct bbox {
    double x1;
    double x2;
    double y1;
    double y2;
} BBox;

struct srbTree {
    Node root;
    double epsilon;
    int size;
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

    tree->root = NULL;
    tree->epsilon = epsilon; 
    tree->size = 0;

    return tree;
}

void rotateLeft(SRBTree tree, Node node) {
    Node right = node->right;
    node->right = right->left;

    if (right->left != NULL) {
        right->left->parent = node;
    }

    right->parent = node->parent;

    if (node == tree->root) {
        tree->root = right;
    } else if (node == node->parent->left) {
        node->parent->left = right;
    } else {
        node->parent->right = right;
    }

    right->left = node;
    node->parent = right;
}

void rotateRight(SRBTree tree, Node node) {
    Node left = node->left;
    node->left = left->right;

    if (left->right != NULL) {
        left->right->parent = node;
    }

    left->parent = node->parent;

    if (node == tree->root) {
        tree->root = left;
    } else if (node == node->parent->right) {
        node->parent->right = left;
    } else {
        node->parent->left = left;
    }

    left->right = node;
    node->parent = left;
}

void fixInsertion(SRBTree t, Node node) {
    Node uncle;
    while (node != t->root && node->parent->color == RED) {
        if (node->parent == node->parent->parent->right) {
            uncle = node->parent->parent->left;
            if (uncle && uncle->color == RED) {
                uncle->color = BLACK;
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rotateRight(t, node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotateLeft(t, node->parent->parent);
            }
        } else {
            uncle = node->parent->parent->right;

            if (uncle && uncle->color == RED) {
                uncle->color = BLACK;
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;	
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    rotateLeft(t, node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotateRight(t, node->parent->parent);
            }
        }

    }
    t->root->color = BLACK;
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

    new_node->left = NULL;
    new_node->right = NULL;

    if (t->root == NULL) {
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

            if (x < aux->x || ((fabs(x - aux->x) < t->epsilon && y < aux->y))) {
                if (aux->left == NULL) {
                    new_node->parent = aux;
                    aux->left = new_node;
                    break;
                } else {
                    aux = aux->left;
                }
            } else {
                if (aux->right == NULL) {
                    new_node->parent = aux;
                    aux->right = new_node;
                    break;
                } else {
                    aux = aux->right;
                }
            }
        }
        fixInsertion(t, new_node);
    }

    t->size++;
    return new_node;
}

Node insertBbSRB(SRBTree t, double mbbX1, double mbbY1, double mbbX2, double mbbY2, SRBTree_elem info) {
    return insertSRB(t, mbbX1, mbbY1, mbbX1, mbbY1, mbbX2, mbbY2, info);
}

void recursiveBbPartSRB(Node n, double x, double y, double x2, double y2, List resultado) {
    if (!n) {
        return;
    }

    if (n->bbox.x1 > x2 || x > n->bbox.x2) {

    } else if (n->bbox.y1 > y2 || y > n->bbox.y1) {

    } else {
        list_append(resultado, n);
    }

    recursiveBbPartSRB(n->left, x, y, x2, y2, resultado);
    recursiveBbPartSRB(n->right, x, y, x2, y2, resultado);
}

void getBbPartSRB(SRBTree t, double x, double y, double w, double h, List resultado) {
    recursiveBbPartSRB(t->root, x, y, x + w, y + h, resultado);
}

void recursiveBbSRB(SRBTree t, Node n, double x, double y, double w, double h, List resultado) {
    if (!n) {
        return;
    }

    Shape outer_rect = rectangle_create(0, x, y, w, h, "", "", 0);
    Shape inside_rect = rectangle_create(0, n->bbox.x1, n->bbox.y1, n->bbox.x2 - n->bbox.x1, n->bbox.y2 - n->bbox.y1, "", "", 0);

    if (shape_inside(outer_rect, inside_rect) == 1) {
        list_append(resultado, n);
    }

    if (n->x < x || (fabs(n->x - x) < t->epsilon && n->y < y)) {
        printf("Trimming left search at [%.2lf, %.2lf]\n", n->x, n->y);
    } else {
        recursiveBbSRB(t, n->left, x, y, w, h, resultado);
    }

    if (n->x > x + w || (fabs(n->x - (x + w)) < t->epsilon && n->y < y + h)) {
        printf("Trimming right search at [%.2lf, %.2lf]\n", n->x, n->y);
    } else {
        recursiveBbSRB(t, n->right, x, y, w, h, resultado);
    }

}

void getBbSRB(SRBTree t, double x, double y, double w, double h, List resultado) {
    recursiveBbSRB(t, t->root, x, y, w, h, resultado);
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
        if (aux->x < xa || (fabs(aux->x - xa) < t->epsilon && aux->y < ya)) {
            aux = aux->left;
        } else {
            aux = aux->right;
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

void fixDelete(SRBTree t, Node node) {

}

SRBTree_elem removeSRB(SRBTree t, double xa, double ya, double *mbbX1, double *mbbY1, double *mbbX2, double *mbbY2) {
//     if (!t) {
//         return NULL;
//     }

//     Node aux = getNodeSRB(t, xa, ya, mbbX1, mbbY1, mbbX2, mbbY2);
//     if (!aux) {
//         return NULL;
//     }

    

//     t->size--;
//     SRBTree_elem elem = aux->elem;
//     free(aux);

//     return elem;

    return NULL;
}

void recursivePrintSRB(Node node, FILE* arq) {
    if (!node) {
        return;
    }

    char name[999];
    char label[999];
    sprintf(name, "node_%p", node);
    sprintf(label, "x=%.4lf y=%.4lf", node->x, node->y);

    fprintf(arq, "\t%s [fillcolor=%s fixedsize=shape label=\"%s\" width=2]\n", name, node->color == RED ? "red" : "black", label);

    char name_left[999];
    char name_right[999];
    if (node->left == NULL) {
        sprintf(name_left, "nilL_%p", node);
        fprintf(arq, "\t%s [fillcolor=black fixedsize=shape label=\"NIL\" width=2]\n", name_left);
    } else {
        sprintf(name_left, "node_%p", node->left);
    }
    if (node->right == NULL) {
        sprintf(name_right, "nilR_%p", node);
        fprintf(arq, "\t%s [fillcolor=black fixedsize=shape label=\"NIL\" width=2]\n", name_right);
    } else {
        sprintf(name_right, "node_%p", node->right);
    }
    fprintf(arq, "\t%s -> %s [label=esquerda] \n\t%s -> %s [label=direita]\n\n", name, name_left, name, name_right);

    recursivePrintSRB(node->left, arq);
    recursivePrintSRB(node->right, arq);
}

void printSRB(SRBTree t, char *nomeArq) {
    FILE* dot = fopen(nomeArq, "w");
    fprintf(dot, "digraph G {\n");
    fprintf(dot, "\tnode [margin=0 fontcolor=white fontsize=14 width=0.5 shape=box style=filled]\n");
    fprintf(dot, "\tedge [fontcolor=grey fontsize=12]\n\n");

    recursivePrintSRB(t->root, dot);
    
    fprintf(dot, "}");
    fclose(dot);
}

void percursoLargura(SRBTree t, FvisitaNo fVisita, void *aux) {
    Queue queue = queue_create(t->size);
    queue_insert(queue, t->root);

    while (!queue_is_empty(queue)) {
        Node node = queue_remove(queue);

        if (node->left) queue_insert(queue, node->left);
        if (node->right) queue_insert(queue, node->right);

        fVisita(node->elem, node->x, node->y, node->bbox.x1, node->bbox.y1, node->bbox.x2, node->bbox.y2, aux);
    }

    queue_destroy(&queue);
}

void recursivoSimetrico(Node node, FvisitaNo fVisita, void* aux) {
    if (node == NULL) {
        return;
    }
    recursivoSimetrico(node->left, fVisita, aux);
    fVisita(node->elem, node->x, node->y, node->bbox.x1, node->bbox.x2, node->bbox.y1, node->bbox.y2, aux);
    recursivoSimetrico(node->right, fVisita, aux);
}

void percursoSimetrico(SRBTree t, FvisitaNo fVisita, void *aux) {
    recursivoSimetrico(t->root, fVisita, aux);
}

void recursivoProfundidade(Node node, FvisitaNo fVisita, void* aux) {
    if (node == NULL) {
        return;
    }
    fVisita(node->elem, node->x, node->y, node->bbox.x1, node->bbox.x2, node->bbox.y1, node->bbox.y2, aux);
    recursivoProfundidade(node->left, fVisita, aux);
    recursivoProfundidade(node->right, fVisita, aux);
}

void percursoProfundidade(SRBTree t, FvisitaNo fVisita, void *aux) {
    recursivoProfundidade(t->root, fVisita, aux);
}

void mbbNodes(SRBTree t, Node no, double* x1, double* y1, double* x2, double* y2) {
    if (!t || !no) {
        return;
    }
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