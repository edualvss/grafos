#include "ligacao.h"
#include "vertice.h"
#include "telaprincipal.h"

#include <QPainter>

#include <cmath>

Ligacao::Ligacao(Vertice *origem, Vertice *destino, int tipoLigacao, int peso) {

    this->origem = origem;
    this->destino = destino;
    this->tipo = tipoLigacao;
    this->peso = peso;
}

QRectF Ligacao::boundingRect() const {

    return QRectF(0,0,1,1);

}

void Ligacao::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {


    painter->setBrush(Qt::black);

    QPointF pontoOrigem = this->origem->pos();
    QPointF pontoDestino = this->destino->pos();

    QLineF linha(pontoOrigem,pontoDestino);

    qreal anguloGraus = linha.angle();
    double seno = sin(anguloGraus * M_PI/180);
    double cosseno = cos(anguloGraus * M_PI/180);
    double catetoOposto = (Vertice::TAM/2) * seno;
    double catetoAdjacente = (Vertice::TAM/2)* cosseno;

    pontoDestino.setY(pontoDestino.y()+catetoOposto);
    pontoDestino.setX(pontoDestino.x()-catetoAdjacente);

    pontoOrigem.setY(pontoOrigem.y()-catetoOposto);
    pontoOrigem.setX(pontoOrigem.x()+catetoAdjacente);

    linha.setPoints(pontoOrigem,pontoDestino);

    // Desenha a linha de um vértice a outro
    painter->drawLine(linha);
//    QRectF retangulo(pontoOrigem,pontoDestino);

//    painter->drawArc(retangulo,16*90,16*270);

    if(tipo == TelaPrincipal::ARCO) {
        qreal acima = anguloGraus + 45;
        qreal abaixo = anguloGraus - 45;

        seno = sin(acima*M_PI/180);
        cosseno = cos(acima*M_PI/180);

        catetoOposto = (Vertice::TAM/4) * seno;
        catetoAdjacente = (Vertice::TAM/4) * cosseno;

        QPointF p1(pontoDestino.x()-catetoAdjacente,pontoDestino.y()+catetoOposto);

        seno = sin(abaixo*M_PI/180);
        cosseno = cos(abaixo*M_PI/180);

        catetoOposto = (Vertice::TAM/4) * seno;
        catetoAdjacente = (Vertice::TAM/4) * cosseno;

        QPointF p2(pontoDestino.x()-catetoAdjacente,pontoDestino.y()+catetoOposto);

        painter->drawLine(pontoDestino,p1);
        painter->drawLine(pontoDestino,p2);
    }

    QPointF pontoCentral = linha.pointAt(0.5);
    painter->drawText(pontoCentral,QString::number(this->peso));

}

/// Não é necessário descobrir o quadrante
/*
int Ligacao::getQuadrante(QPointF origem, QPointF destino) {

    QPointF distancia = origem - destino;

    bool acima = false;
    bool esquerda = false;

    if(distancia.x() > 0) {
        esquerda = true;
    }

    if(distancia.y() > 0) {
        acima = true;
    }

    if(acima) {
        if(esquerda) {
            return 2;
        } else {
            // inclui x == 0 -> 90º ou 270º
            return 1;
        }
    } else {
        // inclui y==0 -> 0º ou 180º
        if(esquerda) {
            return 3;
        } else {
            // inclui x == 0 -> 90º ou 270º
            return 4;
        }
    }

}
*/
