#include "controle.h"

#include "model/planaridade.h"
#include "model/coloracao.h"
#include "model/conexidade.h"
#include "model/caixeiro_viajante/verticeadjacentemaisproximo.h"

//////////////// Construtor ////////////////
Controle::Controle(TelaPrincipal *parent) :
    QObject(parent) {

    this->tela = parent;

    this->matrizAdjacencia = NULL;
    this->numeroVertices = 0;
    connect(tela,SIGNAL(numeroVerticesDefinido(int)),this,SLOT(numeroVerticesDefinido(int)));
    connect(tela,SIGNAL(adicionarLigacao(int,int,int,int)),this,SLOT(adicionarLigacao(int,int,int,int)));
    connect(this->tela,SIGNAL(removerLigacao(int,int,int,int)),this,SLOT(removerLigacao(int,int,int,int)));
    connect(tela,SIGNAL(testarPlanaridade()),this,SLOT(testarPlanaridade()));
    connect(tela,SIGNAL(novo()),this,SLOT(resetarConfiguracoes()));
    connect(tela,SIGNAL(colorir()),this,SLOT(colorirGrafo()));
    connect(tela,SIGNAL(conexidade(int)),this,SLOT(verificarConexidade(int)));
    connect(tela,SIGNAL(caixeiroViajante(int)),this,SLOT(caixeiroViajante(int)));
}
////////////////////////////////

//////////////// Slots ////////////////

void Controle::caixeiroViajante(int verticeInicial) {

    VerticeAdjacenteMaisProximo* vamp =  new VerticeAdjacenteMaisProximo(numeroVertices,matrizAdjacencia,verticeInicial-1);

    char* circuito = vamp->getCircuito();
    int distancia = vamp->getDistancia();

    QString cx = trUtf8("Caixeiro Viajante!\nHeurística: Vértice Adjacente Mais Próximo\n\nDistância: %1").arg(QString::number(distancia));
    QString caminho = "Caminho: "+QString::fromStdString(circuito);

    this->tela->exibirMensagemInformativa(cx,caminho);

    delete[] circuito;
    delete vamp;

}

void Controle::verificarConexidade(int verticeInicial) {

    Conexidade* verificador = new Conexidade(this->matrizAdjacencia,this->numeroVertices,verticeInicial-1);

    // Verifica se é conexo
    if(verificador->isConexo()) {
        int* cores = new int[numeroVertices];
        for(int i = 0; i < numeroVertices; i++ ) {
            cores[i] = Qt::lightGray;
        }
        tela->colorirDesenho(cores);
        delete[] cores;
        this->tela->exibirMensagemInformativa(trUtf8("É conexo"));
    } else { // Se não for busca os sub grafos conexos
        QString exibir = trUtf8("Não é conexo!\n\nSubgrafos conexos!\n\n");
        char** subGrafos = verificador->getSubGrafosConexos();// Obtém Strings com os vértices conexos, separados por vírgula, e sub grafos separados nas posições do vetor
        int qtdSubGrafos = verificador->getQtdSubGrafos();// Obtém a qtd de subgrafos
        int* cores = new int[numeroVertices];
        int contadorCores = 1;
        for(int i = 0; i < qtdSubGrafos; i++) {// Monta uma String para exibir os subgrafos
            exibir += subGrafos[i];
            exibir += '\n';
        }

        QStringList linhas = exibir.split('\n');

        for(int i = 4; i < linhas.size() - 1; i++) {
            QString posAtual = linhas.at(i);
            posAtual.remove(".");
            QStringList valores = posAtual.split(',');
            for(int x = 0; x < valores.size(); x++) {
                cores[valores.at(x).toInt()-1] = contadorCores;
            }
            contadorCores++;
        }

        tela->colorirDesenho(cores);

        delete[] cores;

        tela->exibirMensagemInformativa(exibir);
        for(int i = 0; i < qtdSubGrafos; i++) { // Desaloca a memória alocada para a marcação dos subgrafos
            delete[] subGrafos[i];
        }
        delete[] subGrafos;
    }

    delete verificador;
}

void Controle::colorirGrafo() {

    Coloracao* coloridor = new Coloracao(this->matrizAdjacencia,this->obterVetorGrausVertices(),numeroVertices);
    int* cores = coloridor->getCores();
    int numeroCromatico = coloridor->getNumeroCromatico();

    this->tela->exibirMensagemInformativa(trUtf8("Número Cromático do Grafo: %1").arg(QString::number(numeroCromatico)));

    // Aplicar cores ao desenho
    this->tela->colorirDesenho(cores);

    // Deleta -> Cores, grausAdjacencia, grausSaturacao
    delete coloridor;
}

void Controle::resetarConfiguracoes() {

    this->deletarMatrizAdjacencia();
    this->numeroVertices = 0;

    this->tela->setEntradasEnabled(true);
    this->tela->setLigacoesEnabled(false);
    this->tela->setOpcoesEnabled(false);
    this->tela->resetTela();

}

void Controle::removerLigacao(int tipoLigacao, int origem, int destino, int linhaTabelaLigacoes) {

    this->tela->removerLigacaoTabelaAdjacencia(tipoLigacao,origem,destino);
    this->tela->removerLigacaoTabelaLigacoes(linhaTabelaLigacoes);
    this->tela->removerLigacaoAreaDesenho(origem,destino);
    this->matrizAdjacencia[origem-1][destino-1] = 0;

    if(tipoLigacao == TelaPrincipal::ARESTA) {
        this->matrizAdjacencia[destino-1][origem-1] = 0;
    }

}

void Controle::testarPlanaridade() {

    Planaridade* testador = new Planaridade(numeroVertices,this->obterQuantidadeArestas(),matrizAdjacencia);
    if(testador->isPlanar()) {
        this->tela->exibirMensagemInformativa(trUtf8("O Grafo é planar!"));
    } else {
        this->tela->exibirMensagemInformativa(trUtf8("O Grafo não é planar!"));
    }

    delete testador;

}

void Controle::numeroVerticesDefinido(int numeroVertices) {

    this->numeroVertices = numeroVertices;
    this->tela->setEntradasEnabled(false);
    this->tela->setLigacoesEnabled(true);
    this->tela->setOpcoesEnabled(true);
    this->tela->setMaximoValorEntradas(numeroVertices);

    this->matrizAdjacencia = new int*[numeroVertices];
    for(int i = 0; i < numeroVertices; i++) {
        this->matrizAdjacencia[i] = new int[numeroVertices];
    }
    for(int i = 0; i < numeroVertices; i++) {
        for(int x = 0; x < numeroVertices; x++) {
            matrizAdjacencia[i][x] = 0;
        }
    }

    this->tela->criarCabecalhoMatrizAdjacencia(numeroVertices);
    this->tela->criarVerticesDesenho(numeroVertices);
/*
    this->adicionarLigacao(TelaPrincipal::ARESTA,1,2,2);
    this->adicionarLigacao(TelaPrincipal::ARESTA,1,3,5);
    this->adicionarLigacao(TelaPrincipal::ARESTA,1,4,3);
    this->adicionarLigacao(TelaPrincipal::ARESTA,1,5,6);
    this->adicionarLigacao(TelaPrincipal::ARESTA,2,3,4);
    this->adicionarLigacao(TelaPrincipal::ARESTA,2,4,3);
    this->adicionarLigacao(TelaPrincipal::ARESTA,2,5,4);
    this->adicionarLigacao(TelaPrincipal::ARESTA,3,4,7);
    this->adicionarLigacao(TelaPrincipal::ARESTA,3,5,3);
    this->adicionarLigacao(TelaPrincipal::ARESTA,4,5,3);
*/

    // Grafo do slide - não dirigido -> Fecho Transitivo Direto - Grafo conexo
//    this->adicionarLigacao(TelaPrincipal::ARESTA,1,2);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,1,4);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,2,5);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,3,5);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,3,8);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,4,6);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,5,7);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,5,9);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,6,7);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,8,9);

    // Grafo do slide - dirigido -> Fecho Transitivo Direto & Fecho Transitivo Inverso - Conexo
//    this->adicionarLigacao(TelaPrincipal::ARCO,1,2);
//    this->adicionarLigacao(TelaPrincipal::ARCO,1,5);
//    this->adicionarLigacao(TelaPrincipal::ARCO,2,4);
//    this->adicionarLigacao(TelaPrincipal::ARCO,3,1);
//    this->adicionarLigacao(TelaPrincipal::ARCO,4,3);
//    this->adicionarLigacao(TelaPrincipal::ARCO,4,5);
//    this->adicionarLigacao(TelaPrincipal::ARCO,5,2);

    // Grafo do slide - não dirigido -> Fecho Transitivo Inverso - Conexo
//    this->adicionarLigacao(TelaPrincipal::ARESTA,1,4);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,1,5);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,2,3);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,2,5);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,3,8);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,3,9);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,4,6);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,5,7);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,6,7);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,7,9);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,8,9);

    // Grafo do slide - dirigido -> FTD & FTI - Não conexo
//    this->adicionarLigacao(TelaPrincipal::ARCO,1,2);
//    this->adicionarLigacao(TelaPrincipal::ARCO,1,4);
//    this->adicionarLigacao(TelaPrincipal::ARCO,2,3);
//    this->adicionarLigacao(TelaPrincipal::ARCO,3,7);
//    this->adicionarLigacao(TelaPrincipal::ARCO,4,5);
//    this->adicionarLigacao(TelaPrincipal::ARCO,4,6);
//    this->adicionarLigacao(TelaPrincipal::ARCO,5,1);
//    this->adicionarLigacao(TelaPrincipal::ARCO,5,2);
//    this->adicionarLigacao(TelaPrincipal::ARCO,5,3);
//    this->adicionarLigacao(TelaPrincipal::ARCO,5,7);
//    this->adicionarLigacao(TelaPrincipal::ARCO,6,3);
//    this->adicionarLigacao(TelaPrincipal::ARCO,7,2);
//    this->adicionarLigacao(TelaPrincipal::ARCO,7,6);

    // Grafo 1 da net - dirigido - não conexo
//    this->adicionarLigacao(TelaPrincipal::ARCO,1,2);
//    this->adicionarLigacao(TelaPrincipal::ARCO,2,3);
//    this->adicionarLigacao(TelaPrincipal::ARCO,2,5);
//    this->adicionarLigacao(TelaPrincipal::ARCO,3,6);
//    this->adicionarLigacao(TelaPrincipal::ARCO,4,1);
//    this->adicionarLigacao(TelaPrincipal::ARCO,5,4);
//    this->adicionarLigacao(TelaPrincipal::ARCO,5,6);
//    this->adicionarLigacao(TelaPrincipal::ARCO,6,3);
//    this->adicionarLigacao(TelaPrincipal::ARCO,7,4);
//    this->adicionarLigacao(TelaPrincipal::ARCO,7,5);
//    this->adicionarLigacao(TelaPrincipal::ARCO,7,6);

//    // Grafo 2 da net - dirigido - não conexo
//    this->adicionarLigacao(TelaPrincipal::ARCO,1,2,1);
//    this->adicionarLigacao(TelaPrincipal::ARCO,1,6,1);
//    this->adicionarLigacao(TelaPrincipal::ARCO,1,7,1);
//    this->adicionarLigacao(TelaPrincipal::ARCO,2,1,1);
//    this->adicionarLigacao(TelaPrincipal::ARCO,2,3,1);
//    this->adicionarLigacao(TelaPrincipal::ARCO,3,4,1);
//    this->adicionarLigacao(TelaPrincipal::ARCO,3,5,1);
//    this->adicionarLigacao(TelaPrincipal::ARCO,4,3,1);
//    this->adicionarLigacao(TelaPrincipal::ARCO,6,2,1);
//    this->adicionarLigacao(TelaPrincipal::ARCO,7,1,1);
//    this->adicionarLigacao(TelaPrincipal::ARCO,7,5,1);

//    // Grafo 3 da net - dirigido - caixeiro viajante
//    this->adicionarLigacao(TelaPrincipal::ARCO,1,2,16);
//    this->adicionarLigacao(TelaPrincipal::ARCO,1,3,12);
//    this->adicionarLigacao(TelaPrincipal::ARCO,1,4,18);
//    this->adicionarLigacao(TelaPrincipal::ARCO,1,5,16);
//    this->adicionarLigacao(TelaPrincipal::ARCO,2,1,10);
//    this->adicionarLigacao(TelaPrincipal::ARCO,2,3,18);
//    this->adicionarLigacao(TelaPrincipal::ARCO,2,4,20);
//    this->adicionarLigacao(TelaPrincipal::ARCO,2,5,20);
//    this->adicionarLigacao(TelaPrincipal::ARCO,3,1,18);
//    this->adicionarLigacao(TelaPrincipal::ARCO,3,2,20);
//    this->adicionarLigacao(TelaPrincipal::ARCO,3,4,18);
//    this->adicionarLigacao(TelaPrincipal::ARCO,3,5,16);
//    this->adicionarLigacao(TelaPrincipal::ARCO,4,1,14);
//    this->adicionarLigacao(TelaPrincipal::ARCO,4,2,18);
//    this->adicionarLigacao(TelaPrincipal::ARCO,4,3,10);
//    this->adicionarLigacao(TelaPrincipal::ARCO,4,5,8);
//    this->adicionarLigacao(TelaPrincipal::ARCO,5,1,8);
//    this->adicionarLigacao(TelaPrincipal::ARCO,5,2,12);
//    this->adicionarLigacao(TelaPrincipal::ARCO,5,3,12);
//    this->adicionarLigacao(TelaPrincipal::ARCO,5,4,12);

//    // Grafo Slide - não dirigido - caixeiro viajante
//    this->adicionarLigacao(TelaPrincipal::ARESTA,1,2,23);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,1,3,17);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,1,4,34);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,1,5,44);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,1,6,19);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,2,3,20);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,2,4,37);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,2,5,31);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,2,6,38);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,3,4,17);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,3,5,27);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,3,6,18);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,4,5,10);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,4,6,23);
//    this->adicionarLigacao(TelaPrincipal::ARESTA,5,6,31);


}

void Controle::adicionarLigacao(int tipoLigacao, int origem, int destino, int peso) {

    if(this->verificaLigacaoJaExiste(origem,destino)) {
        this->tela->exibirMensagemInformativa(trUtf8("Ligação já existente!"),trUtf8("Caso queira alterar o tipo\nremova a ligação existente e\ncrie a ligação desejada!"));
        return;
    }

    this->matrizAdjacencia[origem-1][destino-1] = peso;

    if(tipoLigacao == TelaPrincipal::ARESTA) {
        this->matrizAdjacencia[destino-1][origem-1] = peso;
    }

    this->tela->adicionarLigacaoTabelaAdjacencia(tipoLigacao,origem,destino,peso);
    this->tela->adicionarLigacaoTabelaLigacoes(tipoLigacao,origem,destino,peso);
    this->tela->adicionarLigacaoAreaDesenho(tipoLigacao,origem,destino,peso);
}

////////////////////////////////


//////////////// Métodos privados ////////////////
void Controle::deletarMatrizAdjacencia() {
    for(int i = 0; i < numeroVertices; i++) {
        delete[] this->matrizAdjacencia[i];
    }
    delete[] this->matrizAdjacencia;

    this->matrizAdjacencia = NULL;
}

bool Controle::verificaLigacaoJaExiste(int origem, int destino) {

    if(matrizAdjacencia[origem-1][destino-1] > 0) {
        return true;
    }
    return false;

}

int Controle::obterQuantidadeArestas() {

    int contador = 0;

    for(int i = 0; i < numeroVertices - 1; i++) {
        for(int x = i+1; x < numeroVertices; x++) {
            if(matrizAdjacencia[i][x] > 0) {
                contador++;
            }
        }
    }

    return contador;
}

int* Controle::obterVetorGrausVertices() {

    int* graus = new int[numeroVertices];

    for(int i = 0; i < numeroVertices; i++) {
        int contadorAdjacenciasVertice = 0;
        for(int x = 0; x < numeroVertices; x++) {
            if(matrizAdjacencia[i][x] > 0) {
                contadorAdjacenciasVertice++;
            }
        }
        graus[i] = contadorAdjacenciasVertice;
    }

    return graus;

}

////////////////////////////////////////////////

//////////////// Métodos públicos ////////////////
void Controle::iniciarAplicacao() {

    this->tela->show();

}
////////////////////////////////
