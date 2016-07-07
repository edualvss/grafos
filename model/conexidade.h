#ifndef CONEXIDADE_H
#define CONEXIDADE_H

class Conexidade {
private:
    int** matriz;
    int numeroVertices;
    int verticeInicial;

    int* fechoTransitivoDireto;
    int* fechoTransitivoInverso;

    bool* jaVisitados;

    int qtdSubGrafos;

    void calcularFechoTransitivoDireto();
    void calcularFechoTransitivoInverso();

    void resetVetorJaVisitados();

    int getVerticeAtualFTD();
    int getVerticeAtualFTI();

    void setProximosFTD(int verticeAtual);
    void setProximosFTI(int verticeAtual);

    enum{FLAG=-1};
public:
    Conexidade(int** matrizAdjacencia,int numeroVertices,int verticeInicial);
    ~Conexidade();

    bool isConexo();

    char** getSubGrafosConexos();
    int getQtdSubGrafos();

};

#endif // CONEXIDADE_H
