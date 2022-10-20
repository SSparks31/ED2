#include "srbTree.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "list.h"

enum {
    RED = 0,
    BLACK,
    DOUBLEBLACK
};

typedef struct mbb {
    double x1;
    double x2;
    double y1;
    double y2;
} MBB;

struct srbTree {
    Node root;
    Node NIL;
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

    MBB mbb;
};

/* Compara dois valores utilizando o valor de epsilon */
int compareSRB(SRBTree t, double a, double b) {
    if (fabs(a-b) < t->epsilon) {
        return 0;
    }

    return a > b ? 1 : -1;
}

SRBTree createSRB(double epsilon) {
    if (epsilon < 0) {
        return NULL;
    }

    SRBTree tree = malloc(sizeof(struct srbTree));
    Node NIL = calloc(1, sizeof(struct node));

    NIL->color = BLACK;
    NIL->elem = NULL;
    NIL->left = NULL;
    NIL->right = NULL;
    NIL->parent = NULL;

    tree->root = NIL;
    tree->NIL = NIL;
    tree->epsilon = epsilon;

    return tree;
}

void rotateLeft(SRBTree tree, Node node) {
    Node right = node->right;
    node->right = right->left;

    if (right->left != tree->NIL) {
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

    if (left->right != tree->NIL) {
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
            if (uncle->color == RED) {
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

            if (uncle->color == RED) {
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

    new_node->mbb.x1 = mbbX1;
    new_node->mbb.y1 = mbbY1;
    new_node->mbb.x2 = mbbX2;
    new_node->mbb.y2 = mbbY2;

    new_node->elem = info;

    new_node->left = t->NIL;
    new_node->right = t->NIL;

    if (t->root == t->NIL) {
        new_node->color = BLACK;
        new_node->parent = NULL;
        t->root = new_node;
    } else {
        new_node->color = RED;
        Node aux = t->root;

        for (;;) {
            if (compareSRB(t, x, aux->x) == 0 && compareSRB(t, y, aux->y) == 0) { // Informacao ja existe
                free(new_node);
                return NULL;
            }

            if (compareSRB(t, x, aux->x) == -1 || (compareSRB(t, x, aux->x) == 0 && compareSRB(t, y, aux->y) == -1)) {
                if (aux->left == t->NIL) {
                    new_node->parent = aux;
                    aux->left = new_node;
                    break;
                } else {
                    aux = aux->left;
                }
            } else {
                if (aux->right == t->NIL) {
                    new_node->parent = aux;
                    aux->right = new_node;
                    break;
                } else {
                    aux = aux->right;
                }
            }
        }
        if (aux->color == RED) {
            fixInsertion(t, new_node);
        }
    }

    return new_node;
}

Node insertBbSRB(SRBTree t, double mbbX1, double mbbY1, double mbbX2, double mbbY2, SRBTree_elem info) {
    return insertSRB(t, mbbX1, mbbY1, mbbX1, mbbY1, mbbX2, mbbY2, info);
}

void recursiveBbPartSRB(SRBTree t, Node n, double x, double y, double x2, double y2, List resultado) {
    if (n == t->NIL) {
        return;
    }

    if (compareSRB(t, n->mbb.x1, x2) > 0 || compareSRB(t, x, n->mbb.x2) > 0) {

    } else if (compareSRB(t, n->mbb.y1, y2) > 0 || compareSRB(t, y, n->mbb.y1) > 0)  {

    } else {
        list_append(resultado, n);
    }

    recursiveBbPartSRB(t, n->left, x, y, x2, y2, resultado);
    recursiveBbPartSRB(t, n->right, x, y, x2, y2, resultado);
}

void getBbPartSRB(SRBTree t, double x, double y, double w, double h, List resultado) {
    recursiveBbPartSRB(t, t->root, x, y, x + w, y + h, resultado);
}

void recursiveBbSRB(SRBTree t, Node n, double x, double y, double w, double h, List resultado) {
    if (n == t->NIL) {
        return;
    }
    
    if (compareSRB(t, n->mbb.x1, x) >= 0 && compareSRB(t, n->mbb.y1, y) >= 0 && compareSRB(t, n->mbb.x2, x + w) <= 0 && compareSRB(t, n->mbb.y2, y + h) <= 0) {
        list_append(resultado, n);
    }

    if (compareSRB(t, n->x, x) < 0 || (compareSRB(t, n->x, x) == 0 && compareSRB(t, n->y, y) < 0)) {
        printf("Podando busca da sub-arvore esquerda em [%.2lf, %.2lf]\n", n->x, n->y);
    } else {
        recursiveBbSRB(t, n->left, x, y, w, h, resultado);
    }

    if (compareSRB(t, n->x, x + w) > 0 || (compareSRB(t, n->x, x) == 0 && compareSRB(t, n->y, y + h) > 0)) {
        printf("Podando busca da sub-arvore direita em [%.2lf, %.2lf]\n", n->x, n->y);
    } else {
        recursiveBbSRB(t, n->right, x, y, w, h, resultado);
    }

}

void getBbSRB(SRBTree t, double x, double y, double w, double h, List resultado) {
    recursiveBbSRB(t, t->root, x, y, w, h, resultado);
}

SRBTree_elem getInfoSRB(SRBTree t, Node n, double *xa, double *ya, double *mbbX1, double *mbbY1, double *mbbX2, double *mbbY2) {
    if (!t || !n || n == t->NIL) {
        return NULL;
    }

    *xa = n->x;
    *ya = n->y;
    *mbbX1 = n->mbb.x1;
    *mbbY1 = n->mbb.y1;
    *mbbX2 = n->mbb.x2;
    *mbbY2 = n->mbb.y2;

    return n->elem;
}

Node getNodeSRB(SRBTree t, double xa, double ya, double *mbbX1, double *mbbY1, double *mbbX2, double *mbbY2) {
    if (!t || t->root == t->NIL) {
        return NULL;
    }

    Node n = t->root;
    while (compareSRB(t, n->x, xa) != 0 || compareSRB(t, n->y, ya) != 0) {
        if (n == t->NIL) {
            return NULL;
        }

        if (compareSRB(t, n->x, xa) <= 0 || (compareSRB(t, n->x, xa) == 0 && compareSRB(t, n->y, ya) < 0)) {
            n = n->left;
        } else {
            n = n->right;
        }
    }

    return n;    
}

void updateInfoSRB(SRBTree t, Node n, SRBTree_elem i) {
    if (!t || !n || n == t->NIL) {
        return;
    }

    n->elem = i;
}

SRBTree_elem removeSRB(SRBTree t,double xa, double ya, double *mbbX1, double *mbbY1, double *mbbX2, double *mbbY2) {

}

void recursivePrintNodesSRB(SRBTree t, Node n, FILE* arq) {
    if (n->left != t->NIL) {
        recursivePrintNodesSRB(t, n->left, arq);
    } else {
        fprintf(arq, "\tnilL_%p [fillcolor=black fixedsize=shape label=\"NIL\" width=2]\n", n);
    }

    fprintf(arq, "\tnode_%p [fillcolor=%s fixedsize=shape label=\"x=%.4lf\\ny=%.4lf\" width=2]\n", n, n->color == RED ? "red" : "black", n->x, n->y);   

    if (n->right != t->NIL) {
        recursivePrintNodesSRB(t, n->right, arq);
    } else {
        fprintf(arq, "\tnilR_%p [fillcolor=black fixedsize=shape label=\"NIL\" width=2]\n", n);
    }
}

void recursivePrintEdgesSRB(SRBTree t, Node n, FILE* arq) {
    if (n->left != t->NIL) {
        recursivePrintEdgesSRB(t, n->left, arq);
    }

    if (n->left == t->NIL) {
        fprintf(arq, "\tnode_%p -> nilL_%p [label=esquerda] \n", n, n);
    } else {
        fprintf(arq, "\tnode_%p -> node_%p [label=esquerda] \n", n, n->left);
    }
    if (n->right == t->NIL) {
        fprintf(arq, "\tnode_%p -> nilR_%p [label=direita] \n", n, n);
    } else {
        fprintf(arq, "\tnode_%p -> node_%p [label=direita] \n", n, n->right);
    }

    if (n->right != t->NIL) {
        recursivePrintEdgesSRB(t, n->right, arq);
    }
}

void printSRB(SRBTree t, char *nomeArq) {
    FILE* dot = fopen(nomeArq, "w");
    fprintf(dot, "digraph G {\n");
    fprintf(dot, "\tnode [margin=0 fontcolor=white fontsize=30 width=0.5 shape=circle style=filled]\n");
    fprintf(dot, "\tedge [fontcolor=grey fontsize=12]\n\n");

    recursivePrintNodesSRB(t, t->root, dot);
    fprintf(dot, "\n\n");
    recursivePrintEdgesSRB(t, t->root, dot);

    fprintf(dot, "}");
    fclose(dot);
}

void percursoLargura(SRBTree t, FvisitaNo fVisita, void *aux) {
    if (!t || !fVisita || t->root == t->NIL) {
        return;
    }

    List list = list_create();
    list_append(list, t->root);

    while (list_get_size(list) != 0) {
        List_pos aux = list_get_first(list);
        Node n = list_get_elem(list, aux);

        if (n->left != t->NIL) list_append(list, n->left);
        if (n->right != t->NIL) list_append(list, n->right);

        fVisita(n->elem, n->x, n->y, n->mbb.x1, n->mbb.y1, n->mbb.x2, n->mbb.y2, aux);
    }

    list_destroy(&list);
}

void recursivoSimetrico(SRBTree t, Node n, FvisitaNo fVisita, void *aux) {
    if (n == t->NIL) {
        return;
    }

    recursivoSimetrico(t, n->left, fVisita, aux);
    fVisita(n->elem, n->x, n->y, n->mbb.x1, n->mbb.y1, n->mbb.x2, n->mbb.y2, aux);
    recursivoSimetrico(t, n->right, fVisita, aux);
}

void percursoSimetrico(SRBTree t, FvisitaNo fVisita, void *aux) {
    if (!t || !fVisita || t->root == t->NIL) {
        return;
    }

    recursivoSimetrico(t, t->root, fVisita, aux);
}

void recursivoProfundidade(SRBTree t, Node n, FvisitaNo fVisita, void *aux) {
    if (n == t->NIL) {
        return;
    }

    fVisita(n->elem, n->x, n->y, n->mbb.x1, n->mbb.y1, n->mbb.x2, n->mbb.y2, aux);
    recursivoProfundidade(t, n->left, fVisita, aux);
    recursivoProfundidade(t, n->right, fVisita, aux);
}

void percursoProfundidade(SRBTree t, FvisitaNo fVisita, void *aux) {
        if (!t || !fVisita || t->root == t->NIL) {
        return;
    }

    recursivoProfundidade(t, t->root, fVisita, aux);
}

void killSRBNodes(SRBTree t, Node n) {
    if (n == t->NIL) {
        return;
    }

    killSRBNodes(t, n->left);
    killSRBNodes(t, n->right);
    free(n);
}

void killSRB(SRBTree t) {
    if (!t) {
        return;
    }

    killSRBNodes(t, t->root);
    free(t->NIL);
    free(t);
}