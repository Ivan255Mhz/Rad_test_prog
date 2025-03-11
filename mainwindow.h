#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "basket.h"
#include "visualizationwindow.h"

class MainWindow : public QWidget {
    Q_OBJECT

    Basket basket1;
    Basket basket2;

    // Интерфейсные элементы
    QLabel *labelBasket1Red, *labelBasket1Blue, *labelBasket1ProbRed, *labelBasket1ProbBlue;
    QLabel *labelBasket2Red, *labelBasket2Blue, *labelBasket2ProbRed, *labelBasket2ProbBlue;
    QLabel *labelTwoRedProb, *labelTwoBlueProb, *labelMixedProb;
    QPushButton *btnMove1to2, *btnMove2to1, *btnDrawTwo;
    QSpinBox *spinRed1, *spinBlue1, *spinRed2, *spinBlue2;
    QPushButton *btnApply;
    QPushButton *btnVisualize; // Новая кнопка

    VisualizationWindow *visWindow;

    // Начальные значения из командной строки
    int initialRed1, initialBlue1, initialRed2, initialBlue2;

    void initializeBaskets(int argc, char *argv[]);
    void updateUI();
protected:
    void paintEvent(QPaintEvent *event) override;

public:
    MainWindow(int argc, char *argv[], QWidget *parent = nullptr);
    ~MainWindow() {}

private slots:
    void onMove1to2();
    void onMove2to1();
    void onDrawTwo();
    void applyValues();
    void showVisualization(); // Новый слот
};

#endif // MAINWINDOW_H
