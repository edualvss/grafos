#ifndef PLANARIDADE_H
#define PLANARIDADE_H

class Planaridade {

private:
    int numeroVertices;
    int numeroArestas;
    int** matrizAdjacencia;

    bool teste1();
    bool teste2();
    bool possuiCiclosComprimento3();

public:
    Planaridade(int numeroVertices,int numeroArestas, int **matrizAdjacencia);

    bool isPlanar();

};

#endif // PLANARIDADE_H
