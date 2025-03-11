#ifndef VISUALIZATIONWINDOW_H
#define VISUALIZATIONWINDOW_H

#include <QDialog>
#include "basket.h"
#include <QGraphicsView>
#include <QGraphicsScene>

class VisualizationWindow : public QDialog {
    Q_OBJECT

    QGraphicsScene *scene;
    QGraphicsView *view;

    void drawBasket(const Basket &basket, qreal x, qreal y, qreal width);

public:
    VisualizationWindow(const Basket &b1, const Basket &b2, QWidget *parent = nullptr);
    void updateVisualization(const Basket &b1, const Basket &b2);
};

#endif // VISUALIZATIONWINDOW_H
