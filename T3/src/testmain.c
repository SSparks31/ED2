#include <stdio.h>
#include <stdlib.h>

#include "srbTree.h"

int main() {
    SRBTree t = createSRB(0.01);

    insertBbSRB(t, 1, 0, 0, 0, NULL);
    insertBbSRB(t, 2, 0, 0, 0, NULL);
    insertBbSRB(t, 3, 0, 0, 0, NULL);
    insertBbSRB(t, 4, 0, 0, 0, NULL);
    insertBbSRB(t, 5, 0, 0, 0, NULL);
    insertBbSRB(t, 6, 0, 0, 0, NULL);
    insertBbSRB(t, 7, 0, 0, 0, NULL);
    insertBbSRB(t, 8, 0, 0, 0, NULL);
    insertBbSRB(t, 9, 0, 0, 0, NULL);
    insertBbSRB(t, 10, 0, 0, 0, NULL);
    insertBbSRB(t, 11, 0, 0, 0, NULL);
    insertBbSRB(t, 12, 0, 0, 0, NULL);
    insertBbSRB(t, 13, 0, 0, 0, NULL);
    insertBbSRB(t, 14, 0, 0, 0, NULL);
    insertBbSRB(t, 15, 0, 0, 0, NULL);
    insertBbSRB(t, 16, 0, 0, 0, NULL);
    insertBbSRB(t, 17, 0, 0, 0, NULL);
    insertBbSRB(t, 18, 0, 0, 0, NULL);
    insertBbSRB(t, 19, 0, 0, 0, NULL);

    insertBbSRB(t, 1, 1, 0, 0, NULL);
    insertBbSRB(t, 2, 1, 0, 0, NULL);
    insertBbSRB(t, 3, 1, 0, 0, NULL);
    insertBbSRB(t, 4, 1, 0, 0, NULL);
    insertBbSRB(t, 5, 1, 0, 0, NULL);
    insertBbSRB(t, 6, 1, 0, 0, NULL);
    insertBbSRB(t, 7, 1, 0, 0, NULL);
    insertBbSRB(t, 8, 1, 0, 0, NULL);
    insertBbSRB(t, 9, 1, 0, 0, NULL);
    insertBbSRB(t, 10, 1, 0, 0, NULL);
    insertBbSRB(t, 11, 1, 0, 0, NULL);
    insertBbSRB(t, 12, 1, 0, 0, NULL);
    insertBbSRB(t, 13, 1, 0, 0, NULL);
    insertBbSRB(t, 14, 1, 0, 0, NULL);
    insertBbSRB(t, 15, 1, 0, 0, NULL);
    insertBbSRB(t, 16, 1, 0, 0, NULL);
    insertBbSRB(t, 17, 1, 0, 0, NULL);
    insertBbSRB(t, 18, 1, 0, 0, NULL);
    insertBbSRB(t, 19, 1, 0, 0, NULL);

    printSRB(t, "test.dot");
}