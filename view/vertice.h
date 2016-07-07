#ifndef VERTICE_H
#define VERTICE_H

#include <QObject>
#include <QGraphicsItem>

class Vertice : public QObject, public QGraphicsItem {

    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
private:
    int numeroVertice;
    QColor cor;


public:
    static const int TAM = 40;
    explicit Vertice(int numeroVertice,QObject* parent,QColor cor = Qt::lightGray);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setColor(QColor cor);
    QColor getColor();

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:
    void posicaoAlterada();

};

#endif // VERTICE_H
