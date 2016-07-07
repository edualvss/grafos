#include "telaprincipal.h"
#include "ui_telaprincipal.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QMessageBox>

#include "view/vertice.h"
#include "view/ligacao.h"


////////////// Construtor //////////////

TelaPrincipal::TelaPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TelaPrincipal)
{
    ui->setupUi(this);

    this->vertices = new QHash<int,Vertice *>();

    this->cores = new QColor[17];
    this->cores[0] = Qt::white;
    this->cores[1] = Qt::darkGreen;
    this->cores[2] = Qt::cyan;
    this->cores[3] = Qt::red;
    this->cores[4] = Qt::blue;
    this->cores[5] = Qt::green;
    this->cores[6] = Qt::darkGray;
    this->cores[7] = Qt::yellow;
    this->cores[8] = Qt::magenta;
    this->cores[9] = Qt::darkCyan;
    this->cores[10] = Qt::darkBlue;
    this->cores[11] = Qt::darkRed;
    this->cores[12] = Qt::lightGray;
    this->cores[13] = Qt::darkYellow;
    this->cores[14] = Qt::darkMagenta;
    this->cores[15] = Qt::gray;
    this->cores[16] = Qt::black;

    ui->groupBoxLigacoes->setEnabled(false);
    ui->groupBoxOpcoes->setEnabled(false);

    connect(ui->botaoOkNumeroVertices,SIGNAL(clicked()),this,SLOT(botaoOkNumeroVertices()));
    connect(ui->botaoTipoLigacao,SIGNAL(clicked()),this,SLOT(botaoTipoLigacao()));
    connect(ui->botaoAdicionarLigacao,SIGNAL(clicked()),this,SLOT(botaoAdicionarLigacao()));
    connect(ui->botaoTestarPlanaridade,SIGNAL(clicked()),this,SLOT(botaoTestarPlanaridade()));
    connect(ui->actionNovo,SIGNAL(triggered()),this,SLOT(acaoNovaConfiguracao()));
    connect(ui->actionSair,SIGNAL(triggered()),qApp,SLOT(quit()));
    connect(ui->botaoColorirGrafo,SIGNAL(clicked()),this,SLOT(botaoColorirGrafo()));
    connect(ui->botaoConexidade,SIGNAL(clicked()),this,SLOT(botaoConexidade()));
    connect(ui->botaoCaixeiroViajante,SIGNAL(clicked()),this,SLOT(botaoCaixeiroViajante()));

    QAction* acaoExcluirLinhaTabelaLigacoes = new QAction(ui->tabelaLigacoes);
    acaoExcluirLinhaTabelaLigacoes->setShortcut(Qt::Key_Delete);

    this->ui->tabelaLigacoes->addAction(acaoExcluirLinhaTabelaLigacoes);

    connect(acaoExcluirLinhaTabelaLigacoes,SIGNAL(triggered()),this,SLOT(excluirLinhaTabelaLigacoes()));

    cena = new QGraphicsScene();
    this->ui->areaDesenhoGrafo->setScene(cena);

    ui->areaDesenhoGrafo->setCacheMode(QGraphicsView::CacheNone);    


}
//////////////////////////////////////////


////////////// Slots //////////////

void TelaPrincipal::botaoCaixeiroViajante() {
    emit this->caixeiroViajante(ui->inVerticeInicial->value());
}

void TelaPrincipal::botaoConexidade() {
    int verticeInicial = ui->inVerticeInicial->value();
    emit conexidade(verticeInicial);
}

void TelaPrincipal::botaoColorirGrafo() {
    emit colorir();
}

void TelaPrincipal::acaoNovaConfiguracao() {
    emit novo();
}

void TelaPrincipal::excluirLinhaTabelaLigacoes() {

    QList<QTableWidgetItem *> items = this->ui->tabelaLigacoes->selectedItems();

    int origem = 0;
    QString tipo;
    int destino = 0;
    for(int i = 0; i < items.size(); i++) {
        QTableWidgetItem* item = items.at(i);
        if(i % 3 == 0) {
            // Origem
            origem = item->text().toInt();
        } else if(i % 3 == 1) {
            // Tipo
            tipo = item->text();
        } else if( i % 3 == 2) {
            // Destino
            destino = item->text().toInt();
            emit this->removerLigacao(tipo == "Arco"? TelaPrincipal::ARCO: TelaPrincipal::ARESTA,origem,destino,item->row());
            return;
        }
    }

}

void TelaPrincipal::botaoOkNumeroVertices() {

    int numeroVertices = this->ui->inNumeroVertices->value();
    emit this->numeroVerticesDefinido(numeroVertices);

}

void TelaPrincipal::botaoTipoLigacao() {

    if(ui->botaoTipoLigacao->text() == "---->") {
        ui->botaoTipoLigacao->setText("----- ");
    } else {
        ui->botaoTipoLigacao->setText("---->");
    }

}

void TelaPrincipal::botaoAdicionarLigacao() {

    int origem = ui->inOrigem->value();
    int destino = ui->inDestino->value();

    if(origem == destino) {
        QMessageBox msg(this);
        msg.setText(trUtf8("ORIGEM igual ao DESTINO"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();
        return;
    }

    int peso = ui->inPeso->value();

    if(ui->botaoTipoLigacao->text() == "---->") {
        emit this->adicionarLigacao(TelaPrincipal::ARCO,origem,destino,peso);
    } else {
        emit this->adicionarLigacao(TelaPrincipal::ARESTA,origem,destino,peso);
    }

}

void TelaPrincipal::botaoTestarPlanaridade() {
    emit this->testarPlanaridade();
}

void TelaPrincipal::repaintAreaDesenho() {

    this->cena->invalidate();

}

//////////////////////////////////////////

////////////// Metodos públicos //////////////

void TelaPrincipal::colorirDesenho(int *cores) {

    QHashIterator<int,Vertice *> it(*vertices);
    while(it.hasNext()) {
        it.next();
        int chave = it.key();
        Vertice* v = it.value();
        v->setColor(this->cores[cores[chave]]);
    }

}

void TelaPrincipal::criarVerticesDesenho(int numVertices) {

    for(int i = 0; i < numVertices; i++) {
        Vertice* vertice = new Vertice(i+1,this);
        connect(vertice,SIGNAL(posicaoAlterada()),this,SLOT(repaintAreaDesenho()));
        vertice->setPos(20+(i*41),0);
        this->vertices->insert(i,vertice);
        this->cena->addItem(vertice);
    }

}

void TelaPrincipal::resetTela() {

    int qtdLinhasLigacoes = this->ui->tabelaLigacoes->rowCount();
    for(int i = qtdLinhasLigacoes -1; i >= 0; i--) {
        this->removerLigacaoTabelaLigacoes(i);
    }

    int ordemMatriz = ui->tabelaAdjacencia->rowCount();

    for(int i = 0; i < ordemMatriz; i++) {
        for(int x = 0; x < ordemMatriz; x++) {
            delete ui->tabelaAdjacencia->item(i,x);
        }
        delete ui->tabelaAdjacencia->horizontalHeaderItem(i);
        delete ui->tabelaAdjacencia->verticalHeaderItem(i);
    }

    ui->tabelaAdjacencia->setRowCount(0);
    ui->tabelaAdjacencia->setColumnCount(0);

    ui->inNumeroVertices->setValue(1);
    ui->inOrigem->setValue(1);
    ui->inDestino->setValue(1);
    ui->inPeso->setValue(1);

    this->vertices->clear();

    cena->clear();

}

void TelaPrincipal::setEntradasEnabled(bool enabled) {
    this->ui->groupBoxEntradas->setEnabled(enabled);
}

void TelaPrincipal::setLigacoesEnabled(bool enabled) {
    this->ui->groupBoxLigacoes->setEnabled(enabled);
}

void TelaPrincipal::setOpcoesEnabled(bool enabled) {
    this->ui->groupBoxOpcoes->setEnabled(enabled);
}

void TelaPrincipal::setMaximoValorEntradas(int maximo) {
    this->ui->inOrigem->setMaximum(maximo);
    this->ui->inDestino->setMaximum(maximo);
    this->ui->inVerticeInicial->setMaximum(maximo);
}

void TelaPrincipal::exibirMensagemInformativa(QString msg, QString informativo) {

    QMessageBox msgBox(this);
    msgBox.setText(msg);
    msgBox.setInformativeText(informativo);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();

}

void TelaPrincipal::criarCabecalhoMatrizAdjacencia(int numeroVertices) {

    this->ui->tabelaAdjacencia->setColumnCount(numeroVertices);
    this->ui->tabelaAdjacencia->setRowCount(numeroVertices);

    for(int i = 0; i < numeroVertices; i++) {
        QString num = QString::number(i+1);
        QTableWidgetItem* hItem = new QTableWidgetItem(num);
        QTableWidgetItem* vItem = new QTableWidgetItem(num);
        this->ui->tabelaAdjacencia->setHorizontalHeaderItem(i,hItem);
        this->ui->tabelaAdjacencia->setVerticalHeaderItem(i,vItem);
    }

    this->ui->tabelaAdjacencia->resizeColumnsToContents();

    for(int i = 0; i < numeroVertices; i++) {
        for(int x = 0; x < numeroVertices; x++) {
            this->ui->tabelaAdjacencia->setItem(i,x,new QTableWidgetItem());
        }
    }

    for(int i = 0; i < numeroVertices; i++) {
        this->ui->tabelaAdjacencia->item(i,i)->setText("-");
    }

}

void TelaPrincipal::adicionarLigacaoTabelaAdjacencia(int tipo, int origem, int destino, int peso) {

    this->ui->tabelaAdjacencia->item(origem-1,destino-1)->setText( QString::number(peso) );

    if(tipo == TelaPrincipal::ARESTA) {
        this->ui->tabelaAdjacencia->item(destino-1,origem-1)->setText(QString::number(peso));
    }

}

void TelaPrincipal::adicionarLigacaoTabelaLigacoes(int tipo, int origem, int destino,int peso) {

    QTableWidgetItem* ori = new QTableWidgetItem(QString::number(origem));
    QTableWidgetItem* dest = new QTableWidgetItem(QString::number(destino));
    QTableWidgetItem* tip = new QTableWidgetItem();
    QTableWidgetItem* pes = new QTableWidgetItem(QString::number(peso));
    switch(tipo) {
        case TelaPrincipal::ARCO:
            tip->setText("Arco");
            break;
        case TelaPrincipal::ARESTA:
            tip->setText("Aresta");
            break;
    }

    int linha = ui->tabelaLigacoes->rowCount();

    this->ui->tabelaLigacoes->setRowCount(linha+1);

    this->ui->tabelaLigacoes->setItem(linha,0,ori);
    this->ui->tabelaLigacoes->setItem(linha,1,tip);
    this->ui->tabelaLigacoes->setItem(linha,2,dest);
    this->ui->tabelaLigacoes->setItem(linha,3,pes);

}

void TelaPrincipal::adicionarLigacaoAreaDesenho(int tipo, int origem, int destino,int peso) {

    Ligacao* ligacao = new Ligacao(this->vertices->value(origem-1),this->vertices->value(destino-1),tipo,peso);
    ligacao->setData(0,origem);
    ligacao->setData(1,destino);
//    ligacao->setData(2,peso);

    this->cena->addItem(ligacao);
    this->cena->invalidate();

}

void TelaPrincipal::removerLigacaoTabelaAdjacencia(int tipo, int origem, int destino) {

    this->ui->tabelaAdjacencia->item(origem-1,destino-1)->setText("");

    if(tipo == TelaPrincipal::ARESTA) {
        this->ui->tabelaAdjacencia->item(destino-1,origem-1)->setText("");
    }

}

void TelaPrincipal::removerLigacaoTabelaLigacoes(int linha) {
    this->ui->tabelaLigacoes->removeRow(linha);
}

void TelaPrincipal::removerLigacaoAreaDesenho(int origem, int destino) {

    QList<QGraphicsItem *> items = this->cena->items();

    int size = items.size();
    for(int i = 0; i < size; i++) {
        QGraphicsItem* item = items.at(i);
        if( item->data(0).toInt() == origem && item->data(1).toInt() == destino) {
            this->cena->removeItem(item);
            break;
        }
    }
}

//////////////////////////////////////////


////////////// Destrutor //////////////
TelaPrincipal::~TelaPrincipal()
{
    delete ui;
}
//////////////////////////////////////////
