#ifndef _SRB_Tree_H
#define _SRB_Tree_H

/*
    Uma arvore RubroNegra espacial (SRBTree) e' uma arvore Rubro-Negra que associa uma coordenada no plano (ancora)
a uma dada informacao e tambem define o menor retangulo envolvente (minimum bounding box -- MBB) que delimita 
tal informacao. Tipicamente, a ancora esta' dentro (incluindo fronteira) da MBB.
    O MBB e' definido por 2 pontos (x1,y1) e (x2,y2), diagonal que determina o retangulo envolvente: x1 <= x2 e y1 <= y2.

    A chave de busca e´ a coordenada da ancora.

    A ordem da chave de busca e' definido como, a seguir. Sejam ch1 e ch2 duas chaves de busca.
Define-se que ch1 < ch2, se:

        * ch1.x < ch2.x    
        OU
        * ch1.x == ch2.x  AND ch1.y < ch2.y

caso contrario, ch1 == ch2

    Associa-se uma precisao epsilon `a arvore. Esta precisao e' usada para determinar quando dois numeros
reais (da chave de busca) devem ser considerados como iguais. Assim v == w, se |v - w| <= epsilon.

    Algumas operacoes de busca retorna o no´ da arvore onde uma determinada informacao esta' armazenada. 
Os dados referentes a este no´ podem ser obtidos por outras operacoes, desde que estes nos continuem
validos. Qualquer remocao posterior faz com que nos (Node) retornados previamente devam ser considerados
invalidos. 
*/

#include "list.h"

typedef struct srbTree*  SRBTree; 
typedef struct node* Node;
typedef void* SRBTree_elem;


typedef void (*FvisitaNo) (SRBTree_elem i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void *aux);
/*
 * Processa a informacao i associada a um no' da arvore, cuja ancora
 * e' o ponto (x,y), bounding box (mbbX1,mbbY1) -- (mbbX2,mbbY2). 
 * O parametro aux aponta para conjunto de dados 
 * (provavelmente um registro) que sao compartilhados entre as 
 * sucessivas invocacoes a esta funcao.
 */

SRBTree createSRB(double epsilon);
/*
  Retorna uma ´arvore vazia, com precisao epsilon.
  Retorna NULL caso nao tenha conseguido criar a arvore.
 */

Node insertSRB(SRBTree t, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, SRBTree_elem info);
/*
  Insere a informacao info na arvore t, associada a coordenada (x,y), sendo delimitada pelo retangulo definido
  pela diagonal (mbbX1,mbbY1) -- (mbbX2,mbbY2).
  Retorna o no´ onde foi inserida a informacao; NULL, caso ja exista informacao com a mesma chave (veja acima)
  inserida.
 */

Node insertBbSRB(SRBTree t, double mbbX1, double mbbY1, double mbbX2, double mbbY2, SRBTree_elem info);
/*
 Equivalente a insertSRB(t, mbbX1, mbbY1,  mbbX1, mbbY1,  mbbX2, mbbY2,  info)
 */

void getBbPartSRB(SRBTree t, double x, double y, double w, double h, List resultado);
/*
  Insere na lista resultado os nos da arvore (Node) que seu retangulo envolvente possua alguma
  interseccao com a regiao retangula de ancora (x,y), largura w e altura h.
 */

void getBbSRB(SRBTree t, double x, double y, double w, double h, List resultado);
/*
 Similar a getBbPartSRB, porem seleciona apenas os nos cujos retangulos envolventes estejam
 inteiramente dentro da regiao.
 */


SRBTree_elem getInfoSRB(SRBTree t, Node n, double *xa, double *ya, double *mbbX1, double *mbbY1, double *mbbX2, double *mbbY2);
/*
  Retorna a informacao associada ao no´ n, sua ancora (xa,ya) e o retangulo envolvente (mbbX1,mbbY1) -- (mbbX2,mbbY2). 
  Este no´ deve ser um no´ valido (veja acima).
 */

Node getNodeSRB(SRBTree t, double xa, double ya, double *mbbX1, double *mbbY1, double *mbbX2, double *mbbY2);
/*
  Retorna o no da arvore associado a ancora (xa,ya) e o retangulo envolvente; NULL, se tal ancora nao existir.
 */

void updateInfoSRB(SRBTree t, Node n, SRBTree_elem i);
/*
  Altera a informacao associada ao no n que deve ser existente e valido. 
  A ancora e o MBB deste no nao sao alterados, portanto, a informacao deve permanecer compativel a estes dois
valores.
 */


SRBTree_elem removeSRB(SRBTree t,double xa, double ya, double *mbbX1, double *mbbY1, double *mbbX2, double *mbbY2);
/*
  Remove o no' da arvore cuja chave e´ a coordenada (xa,ya). Qualquer no' da arvore retornado por operacoes anteriores
  deve ser considerado invalido.
  Retorna a informacao que estava associada a tal no' (ou NULL, se nao encontrado), bem como o seu retangulo envolvente.
 */

void printSRB(SRBTree t, char *nomeArq);
/*
  "Desenha" (no formato dot) a arvore no arquivo nomeArq.
  Veja: https://graphviz.org/
 */


/*
  As proximas operacoes percorrem a arvore em largura/profundidade/simetrico.
  Invoca a funcao fVisita (veja descricao acima) em cada no´ visitado
 */

void percursoLargura(SRBTree t, FvisitaNo fVisita, void *aux);

void percursoSimetrico(SRBTree t, FvisitaNo fVisita, void *aux);

void percursoProfundidade(SRBTree t, FvisitaNo fVisita, void *aux);

/*
  Desaloca todos os recursos usados pela arvore t.
 */
void killSRB(SRBTree t);

#endif