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
    BBox nodes_bbox;
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

    new_node->nodes_bbox = new_node->bbox;

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

    Node aux = new_node->parent;
    while (aux) {
        if (new_node->bbox.x1 < aux->nodes_bbox.x1) {
            aux->nodes_bbox.x1 = new_node->bbox.x1;
        }
        if (new_node->bbox.y1 < aux->nodes_bbox.y1) {
            aux->nodes_bbox.y1 = new_node->bbox.y1;
        }
        if (new_node->bbox.x2 > aux->nodes_bbox.x2) {
            aux->nodes_bbox.x2 = new_node->bbox.x2;
        }
        if (new_node->bbox.y2 > aux->nodes_bbox.y2) {
            aux->nodes_bbox.y2 = new_node->bbox.y2;
        }
        aux = aux->parent;
    }

    t->size++;
    return new_node;
}

Node insertBbSRB(SRBTree t, double mbbX1, double mbbY1, double mbbX2, double mbbY2, SRBTree_elem info) {
    return insertSRB(t, mbbX1, mbbY1, mbbX1, mbbY1, mbbX2, mbbY2, info);
}

void recursiveBbPartSRB(Node node, Shape outer_rect, Shape outer_points[4], List resultado) {
    if (!node) {
        return;
    }

    Shape node_rect = rectangle_create(0, node->bbox.x1, node->bbox.y1, node->bbox.x2 - node->bbox.x1, node->bbox.y2 - node->bbox.y1, "", "", 0);
    Shape node_points[4];
    node_points[0] = point_create(0, node->bbox.x1, node->bbox.y1, "", "", 0);
    node_points[1] = point_create(0, node->bbox.x1, node->bbox.y2, "", "", 0);
    node_points[2] = point_create(0, node->bbox.x2, node->bbox.y1, "", "", 0);
    node_points[3] = point_create(0, node->bbox.x2, node->bbox.y2, "", "", 0);
    
    if (
        shape_inside(outer_rect, node_points[0]) ||
        shape_inside(outer_rect, node_points[1]) ||
        shape_inside(outer_rect, node_points[2]) ||
        shape_inside(outer_rect, node_points[3]) ||
        shape_inside(node_rect, outer_points[0]) ||
        shape_inside(node_rect, outer_points[1]) ||
        shape_inside(node_rect, outer_points[2]) ||
        shape_inside(node_rect, outer_points[3]) ||
        intersect()
    ) {
        list_append(resultado, node);
    }

    double* coordinates = shape_get_coordinates(outer_rect);  
    double x1 = coordinates[0];
    double y1 = coordinates[1];
    double x2 = coordinates[2] + x1;
    double y2 = coordinates[3] + y1;

    recursiveBbPartSRB(node->left, outer_rect, outer_points, resultado);
    recursiveBbPartSRB(node->right, outer_rect, outer_points, resultado);
}

void getBbPartSRB(SRBTree t, double x, double y, double w, double h, List resultado) {
    Shape outer_rect = rectangle_create(0, x, y, w, h, "", "", 0);
    
    Shape outer_points[4];
    outer_points[0] = point_create(0, x, y, "", "", 0);
    outer_points[1] = point_create(0, x + w, y, "", "", 0);
    outer_points[2] = point_create(0, x, y + h, "", "", 0);
    outer_points[3] = point_create(0, x + w, y + h, "", "", 0);
    
    recursiveBbPartSRB(t->root, outer_rect, outer_points, resultado);
    shape_destroy(&outer_rect);
    for (int i = 0; i < 4; ++i) {
        shape_destroy(outer_points + i);
    }
}

void recursiveBbSRB(SRBTree t, Node node, Shape outer_rect, List resultado) {
    if (!t || !node) {
        return;
    }

    Shape node_rect = rectangle_create(0, node->bbox.x1, node->bbox.y1, node->bbox.x2 - node->bbox.x1, node->bbox.y2 - node->bbox.y1, "", "", 0);
    if (shape_inside(outer_rect, node_rect)) {
        list_append(resultado, node);
    }
    shape_destroy(&node_rect);

    double* coordinates = shape_get_coordinates(outer_rect);  
    double x1 = coordinates[0];
    double y1 = coordinates[1];
    double x2 = coordinates[2] + x1;
    double y2 = coordinates[3] + y1;

    if (node->x > x1 || (fabs(node->x - x1) < t->epsilon && node->y > y1)) {
        recursiveBbSRB(t, node->left, outer_rect, resultado);
    }

    if (node->x < x2 || (fabs(node->x - x1) < t->epsilon && node->y < y2)) {
        recursiveBbSRB(t, node->right, outer_rect, resultado);
    }

}

void getBbSRB(SRBTree t, double x, double y, double w, double h, List resultado) {
    Shape outer_rect = rectangle_create(0, x, y, w, h, "", "", 0);
    recursiveBbSRB(t, t->root, outer_rect, resultado);
    shape_destroy(&outer_rect);
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

    *x1 = no->nodes_bbox.x1;
    *y1 = no->nodes_bbox.y1;
    *x2 = no->nodes_bbox.x2;
    *y2 = no->nodes_bbox.y2;
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