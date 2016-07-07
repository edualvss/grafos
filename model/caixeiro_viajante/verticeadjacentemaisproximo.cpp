#include "verticeadjacentemaisproximo.h"

#include <cstdio>
#include <cstring>

VerticeAdjacenteMaisProximo::VerticeAdjacenteMaisProximo(int numeroVertices, int **matrizAdjacencia,int verticeInicial) {

    this->numeroVertices = numeroVertices;
    this->verticeInicial = verticeInicial;
    this->matrizAdjacencia = matrizAdjacencia;

    this->visitados = new int[numeroVertices];
    this->caminho = new int[numeroVertices+1];
    for(int i = 0; i < numeroVertices; i++) {
        visitados[i] = 0;
        caminho[i] = -1;
    }
    caminho[numeroVertices] = -1;
}

void VerticeAdjacenteMaisProximo::encontrarCircuito() {
    int verticeAtual = verticeInicial;

    this->visitados[verticeAtual] = -1;

    int i = 0;
    do {
        int proximo = this->getProximoVertice(verticeAtual);

        if(proximo != -1) {
            this->visitados[proximo] = matrizAdjacencia[verticeAtual][proximo];
        } else {
            this->visitados[verticeInicial] = matrizAdjacencia[verticeAtual][verticeInicial];
            this->caminho[numeroVertices] = verticeInicial;
        }
        this->caminho[i++] = verticeAtual;
        verticeAtual = proximo;

    }while(verticeAtual != -1);

}

int VerticeAdjacenteMaisProximo::getDistancia() {
    int dist = 0;

    for(int i = 0; i < numeroVertices; i++) {
        dist += visitados[i];
    }
    return dist;
}

char* VerticeAdjacenteMaisProximo::getCircuito() {

    encontrarCircuito();

    int size = 0;
    for(int i = 0; i < numeroVertices + 1; i++) {
        char buffer[5];
        size += sprintf(buffer,"%d",caminho[i]+1);
    }

    char* circuito = new char[size+numeroVertices+2];
    circuito[0] = '\0';
    for(int i = 0; i < numeroVertices + 1; i++) {
        char buf[4];
        sprintf(buf,"%d,",caminho[i]+1);
        strcat(circuito,buf);
    }
    circuito[size+numeroVertices] = '.';
    circuito[size+numeroVertices+1] = '\0';



    return circuito;

}

int VerticeAdjacenteMaisProximo::getProximoVertice(int atual) {

    int menorPeso = __INT_MAX__;
    int indice = -1;

    for(int i = 0; i < numeroVertices; i++) {
        if(matrizAdjacencia[atual][i] > 0 && visitados[i] == 0) {
            if(matrizAdjacencia[atual][i] < menorPeso) {
                menorPeso = matrizAdjacencia[atual][i];
                indice = i;
            }
        }
    }

    return indice;

}

VerticeAdjacenteMaisProximo::~VerticeAdjacenteMaisProximo() {
    delete[] this->visitados;
    delete[] this->caminho;
}
