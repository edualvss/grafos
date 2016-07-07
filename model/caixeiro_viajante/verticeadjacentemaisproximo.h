#ifndef VERTICEADJACENTEMAISPROXIMO_H
#define VERTICEADJACENTEMAISPROXIMO_H

class VerticeAdjacenteMaisProximo {
    int numeroVertices;
    int verticeInicial;
    int** matrizAdjacencia;

    int* visitados;
    int* caminho;

    int getProximoVertice(int atual);

    void encontrarCircuito();

public:
    VerticeAdjacenteMaisProximo(int numeroVertices,int** matrizAdjacencia, int verticeInicial);

    char* getCircuito();
    int getDistancia();

    ~VerticeAdjacenteMaisProximo();

};

#endif // VERTICEADJACENTEMAISPROXIMO_H
