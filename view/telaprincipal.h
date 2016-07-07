#ifndef TELAPRINCIPAL_H
#define TELAPRINCIPAL_H

#include <QMainWindow>
#include <QHash>

namespace Ui {
class TelaPrincipal;
}

class QGraphicsScene;
class Vertice;
class Ligacao;

class TelaPrincipal : public QMainWindow {
    Q_OBJECT

public:

    enum {ARESTA,ARCO};

    explicit TelaPrincipal(QWidget *parent = 0);
    ~TelaPrincipal();

    void setEntradasEnabled(bool enabled);
    void setLigacoesEnabled(bool enabled);
    void setOpcoesEnabled(bool enabled);
    void setMaximoValorEntradas(int maximo);
    void criarCabecalhoMatrizAdjacencia(int numeroVertices);

    void criarVerticesDesenho(int numVertices);

    void exibirMensagemInformativa(QString msg,QString informativo = QString());

    void adicionarLigacaoTabelaLigacoes(int tipo,int origem,int destino, int peso);
    void adicionarLigacaoTabelaAdjacencia(int tipo,int origem,int destino, int peso);
    void adicionarLigacaoAreaDesenho(int tipo,int origem,int destino, int peso);

    void removerLigacaoTabelaLigacoes(int linha);
    void removerLigacaoTabelaAdjacencia(int tipo, int origem, int destino);
    void removerLigacaoAreaDesenho(int origem,int destino);

    void colorirDesenho(int* cores);

    void resetTela();

signals:
    void numeroVerticesDefinido(int numero);
    void adicionarLigacao(int tipo,int origem,int destino,int peso);
    void removerLigacao(int tipo,int origem, int destino, int linhaTabela);
    void testarPlanaridade();
    void novo();
    void colorir();
    void conexidade(int verticeInicial);
    void caixeiroViajante(int verticeInicial);

private slots:
    void botaoOkNumeroVertices();
    void botaoTipoLigacao();
    void botaoAdicionarLigacao();
    void botaoTestarPlanaridade();
    void botaoConexidade();
    void acaoNovaConfiguracao();
    void botaoColorirGrafo();
    void botaoCaixeiroViajante();
    void excluirLinhaTabelaLigacoes();
    void repaintAreaDesenho();
    
private:

    QColor* cores;
    Ui::TelaPrincipal *ui;
    QGraphicsScene* cena;
    QHash<int,Vertice *> *vertices;

};

#endif // TELAPRINCIPAL_H
