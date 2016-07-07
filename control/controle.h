#ifndef CONTROLE_H
#define CONTROLE_H

#include <QObject>

#include "view/telaprincipal.h"

class Controle : public QObject {
    Q_OBJECT

private:
    TelaPrincipal* tela;

    int numeroVertices;

    int **matrizAdjacencia;

    void deletarMatrizAdjacencia();
    bool verificaLigacaoJaExiste(int origem,int destino);
    int obterQuantidadeArestas();
    int* obterVetorGrausVertices();

public:
    explicit Controle(TelaPrincipal *parent = 0);

    void iniciarAplicacao();
    
signals:
    
private slots:
    
    void numeroVerticesDefinido(int numeroVertices);
    void adicionarLigacao(int tipoLigacao,int origem,int destino,int peso);
    void removerLigacao(int tipoLigacao,int origem,int destino, int linhaTabelaLigacoes);
    void testarPlanaridade();
    void resetarConfiguracoes();
    void colorirGrafo();
    void verificarConexidade(int verticeInicial);
    void caixeiroViajante(int verticeInicial);

};

#endif // CONTROLE_H
