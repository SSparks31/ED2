#include <stdio.h>
#include <stdlib.h>

#include "srbTree.h"
#include "geo.h"
#include "qry.h"


int main() {
    SRBTree t = createSRB(0.001);
    
    geo_parser("../Testes", "../Output", "04-mar-grande.geo", t);

    removeSRB(t, 53.52,  195.52, NULL, NULL, NULL, NULL);
    removeSRB(t, 100.61, 196.61, NULL, NULL, NULL, NULL);
    removeSRB(t, 190.94, 256.53, NULL, NULL, NULL, NULL);
    removeSRB(t, 196.36,  91.53, NULL, NULL, NULL, NULL);
    removeSRB(t, 262.14, 140.19, NULL, NULL, NULL, NULL);
    removeSRB(t, 266.19,  73.01, NULL, NULL, NULL, NULL);
    removeSRB(t, 272.67, 150.78, NULL, NULL, NULL, NULL);
    removeSRB(t, 343.52, 235.69, NULL, NULL, NULL, NULL);
    removeSRB(t, 367.33, 204.13, NULL, NULL, NULL, NULL);
    removeSRB(t, 380.48, 264.57, NULL, NULL, NULL, NULL);


    printSRB(t, "test.dot");
}