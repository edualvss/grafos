#ifndef LIGACAO_H
#define LIGACAO_H

#include <QGraphicsItem>

class Vertice;

class Ligacao : public QGraphicsItem {
private:

    Vertice* origem;
    Vertice* destino;
    int tipo;
    int peso;

//    int getQuadrante(QPointF origem, QPointF destino);

public:
    Ligacao(Vertice* origem,Vertice* destino,int tipoLigacao,int peso);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif // LIGACAO_H
