#include "visualizationwindow.h"
#include <QGraphicsEllipseItem>
#include <QBrush>

VisualizationWindow::VisualizationWindow(const Basket &b1, const Basket &b2, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Визуализация корзин");
    resize(800, 400);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(view);
    setLayout(layout);

    updateVisualization(b1, b2);
}

void VisualizationWindow::drawBasket(const Basket &basket, qreal x, qreal y, qreal width) {
    qreal ballSize = 30;
    qreal spacing = 10;
    qreal totalWidth = width - 2*spacing;
    qreal currentX = x + spacing;
    qreal currentY = y + spacing;

    // Рисуем красные шары
    for (int i = 0; i < basket.GetRed(); ++i) {
        QGraphicsEllipseItem *item = scene->addEllipse(currentX, currentY, ballSize, ballSize);
        item->setBrush(Qt::red);
        currentX += ballSize + spacing;
        if (currentX + ballSize > x + width) {
            currentX = x + spacing;
            currentY += ballSize + spacing;
        }
    }

    // Рисуем синие шары
    for (int i = 0; i < basket.GetBlue(); ++i) {
        QGraphicsEllipseItem *item = scene->addEllipse(currentX, currentY, ballSize, ballSize);
        item->setBrush(Qt::blue);
        currentX += ballSize + spacing;
        if (currentX + ballSize > x + width) {
            currentX = x + spacing;
            currentY += ballSize + spacing;
        }
    }
}

void VisualizationWindow::updateVisualization(const Basket &b1, const Basket &b2) {
    scene->clear();

    // Рисуем корзину 1
    scene->addRect(50, 50, 300, 300, QPen(Qt::black));
    scene->addText("Корзина 1")->setPos(50, 30);
    drawBasket(b1, 50, 50, 300);

    // Рисуем корзину 2
    scene->addRect(450, 50, 300, 300, QPen(Qt::black));
    scene->addText("Корзина 2")->setPos(450, 30);
    drawBasket(b2, 450, 50, 300);
}
