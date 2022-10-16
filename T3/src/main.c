#include <stdio.h>

#include <unistd.h>
#include <getopt.h>

#include "svg.h"

#include "geo.h"
#include "qry.h"

#define DEGRADATION 0.5

int main(int argc, char* argv[]) {
    char option;
    
    char* BED = "./";
    char* BSD = NULL;
    char* geo_name = NULL;
    char* qry_name = NULL;

    while ((option = getopt(argc, argv, "e:f:o:q:")) != -1) {
        switch (option) {
        case 'e':
            BED = optarg;
            break;
        
        case 'f':
            geo_name = optarg;
            break;
        
        case 'o':
            BSD = optarg;
            break;
        
        case 'q':
            qry_name = optarg;
            break;
        
        default:
            printf("Opcao nao reconhecida\n");
            break;
        }
    }
    if (!geo_name) {
        printf("Opcao `f` e obrigatoria\n");
        return -1;
    }

    if (!BSD) {
        printf("Opcao `o` e obrigatoria\n");
        return -1;
    }

    // XyyTree boats = xyytree_create(DEGRADATION);
    
    geo_parser(BED, BSD, geo_name, boats);

    qry_parser(BED, BSD, geo_name, qry_name, boats);

    xyytree_destroy(&boats);

    return 0;
}
