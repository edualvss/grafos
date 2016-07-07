#include "vertice.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>


Vertice::Vertice(int numeroVertice,QObject *parent, QColor cor) : QObject(parent) {
    this->numeroVertice = numeroVertice;
    this->cor = cor;
    this->setFlag(QGraphicsItem::ItemIsMovable);
}

QRectF Vertice::boundingRect() const {

    return QRectF(-TAM/2,-TAM/2,TAM,TAM);

}

QPainterPath Vertice::shape() const {

    QPainterPath path;
    path.addEllipse(-TAM/2,-TAM/2,TAM,TAM);
    return path;

}


void Vertice::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {

    painter->setBrush(cor);
    painter->drawEllipse(-TAM/2,-TAM/2,TAM,TAM);

    painter->setBrush(Qt::black);
    painter->drawText(-4,4,QString::number(this->numeroVertice));

}

void Vertice::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

    this->setX( this->x() + event->pos().x());
    this->setY( this->y() + event->pos().y());

    emit posicaoAlterada();

}

void Vertice::setColor(QColor cor) {
    this->cor = cor;
}

QColor Vertice::getColor() {
    return this->cor;
}
