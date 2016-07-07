#ifndef COLORACAO_H
#define COLORACAO_H

class Coloracao {
private:
    int** matrizAdjacencia;
    int* grausAdjacencia;
    int* grausSaturacao;
    int* cores;
    int numeroVertices;

    int numeroCromatico;

    int getVerticeMaiorGrauAdjacencia();
    bool verticeJaColorido(int numVertice);
    void calcularGrausSaturacao();
    int getProximoAColorir();
    int getMenorCorPossivel(int verticeAColorir);
    bool todosColoridos();

    void colorir();

public:
    Coloracao(int** matrizAdjacencia,int* grausAdjacencia,int numeroVertices);
    ~Coloracao();

    int* getCores();
    int getNumeroCromatico();
};

#endif // COLORACAO_H
