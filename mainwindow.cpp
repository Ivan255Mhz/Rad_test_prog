#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QSpinBox>


void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    QPixmap background(":/img/blc.jpg");

    QPixmap scaledBackground = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);


    painter.drawPixmap(0, 0, scaledBackground);


    QWidget::paintEvent(event);
}


MainWindow::MainWindow(int argc, char *argv[], QWidget *parent)
    : QWidget(parent),
    basket1(1, 1),
    basket2(1, 1),
    initialRed1(1),
    initialBlue1(1),
    initialRed2(1),
    initialBlue2(1),
    visWindow(nullptr)
{
    initializeBaskets(argc, argv);
    setAutoFillBackground(false);
    setAttribute(Qt::WA_StyledBackground, false);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Группа корзины 1
    QGroupBox *group1 = new QGroupBox("Корзина 1");
    QFormLayout *form1 = new QFormLayout;
    labelBasket1Red = new QLabel;
    labelBasket1Blue = new QLabel;
    labelBasket1ProbRed = new QLabel;
    labelBasket1ProbBlue = new QLabel;
    form1->addRow("Красных:", labelBasket1Red);
    form1->addRow("Синих:", labelBasket1Blue);
    form1->addRow("Вероятность красного:", labelBasket1ProbRed);
    form1->addRow("Вероятность синего:", labelBasket1ProbBlue);
    group1->setLayout(form1);

    // Группа корзины 2
    QGroupBox *group2 = new QGroupBox("Корзина 2");
    QFormLayout *form2 = new QFormLayout;
    labelBasket2Red = new QLabel;
    labelBasket2Blue = new QLabel;
    labelBasket2ProbRed = new QLabel;
    labelBasket2ProbBlue = new QLabel;
    form2->addRow("Красных:", labelBasket2Red);
    form2->addRow("Синих:", labelBasket2Blue);
    form2->addRow("Вероятность красного:", labelBasket2ProbRed);
    form2->addRow("Вероятность синего:", labelBasket2ProbBlue);
    group2->setLayout(form2);

    // Кнопки управления
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    btnMove1to2 = new QPushButton("Переложить из 1 в 2");
    btnMove2to1 = new QPushButton("Переложить из 2 в 1");
    btnDrawTwo = new QPushButton("Извлечь 2 шара");
    btnVisualize = new QPushButton("Показать визуализацию");
    buttonsLayout->addWidget(btnMove1to2);
    buttonsLayout->addWidget(btnMove2to1);
    buttonsLayout->addWidget(btnDrawTwo);
    buttonsLayout->addWidget(btnVisualize);

    // Группа вероятностей
    QGroupBox *groupTwo = new QGroupBox("Вероятности при извлечении 2 шаров");
    QFormLayout *formTwo = new QFormLayout;
    labelTwoRedProb = new QLabel;
    labelTwoBlueProb = new QLabel;
    labelMixedProb = new QLabel;
    formTwo->addRow("Два красных:", labelTwoRedProb);
    formTwo->addRow("Два синих:", labelTwoBlueProb);
    formTwo->addRow("Один красный и один синий:", labelMixedProb);
    groupTwo->setLayout(formTwo);

    // Группа настроек
    QGroupBox *settingsGroup = new QGroupBox("Настройка количества шаров");
    QFormLayout *settingsLayout = new QFormLayout;
    spinRed1 = new QSpinBox;
    spinRed1->setRange(0, 1000);
    spinRed1->setValue(initialRed1);
    settingsLayout->addRow("Красные в корзине 1:", spinRed1);

    spinBlue1 = new QSpinBox;
    spinBlue1->setRange(0, 1000);
    spinBlue1->setValue(initialBlue1);
    settingsLayout->addRow("Синие в корзине 1:", spinBlue1);

    spinRed2 = new QSpinBox;
    spinRed2->setRange(0, 1000);
    spinRed2->setValue(initialRed2);
    settingsLayout->addRow("Красные в корзине 2:", spinRed2);

    spinBlue2 = new QSpinBox;
    spinBlue2->setRange(0, 1000);
    spinBlue2->setValue(initialBlue2);
    settingsLayout->addRow("Синие в корзине 2:", spinBlue2);

    btnApply = new QPushButton("Применить значения");
    settingsLayout->addWidget(btnApply);
    settingsGroup->setLayout(settingsLayout);

    logTextEdit = new QPlainTextEdit;
    logTextEdit->setReadOnly(true);
    logTextEdit->setMaximumBlockCount(3);
    logTextEdit->setFixedHeight(60);

    mainLayout->addWidget(group1);
    mainLayout->addWidget(group2);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(groupTwo);
    mainLayout->addWidget(settingsGroup);
    mainLayout->addWidget(logTextEdit);

    // Подключение сигналов
    connect(btnMove1to2, &QPushButton::clicked, this, &MainWindow::onMove1to2);
    connect(btnMove2to1, &QPushButton::clicked, this, &MainWindow::onMove2to1);
    connect(btnDrawTwo, &QPushButton::clicked, this, &MainWindow::onDrawTwo);
    connect(btnApply, &QPushButton::clicked, this, &MainWindow::applyValues);
    connect(btnVisualize, &QPushButton::clicked, this, &MainWindow::showVisualization);

    updateUI();
}

void MainWindow::initializeBaskets(int argc, char *argv[]) {
    if (argc >= 5) {
        bool ok;
        int r1 = QString(argv[1]).toInt(&ok);
        if (ok && r1 >= 0) initialRed1 = r1;
        int b1 = QString(argv[2]).toInt(&ok);
        if (ok && b1 >= 0) initialBlue1 = b1;
        int r2 = QString(argv[3]).toInt(&ok);
        if (ok && r2 >= 0) initialRed2 = r2;
        int b2 = QString(argv[4]).toInt(&ok);
        if (ok && b2 >= 0) initialBlue2 = b2;
        logAction("Корзины инициализированы через командную строку");
    }

    basket1 = Basket(initialRed1, initialBlue1);
    basket2 = Basket(initialRed2, initialBlue2);
}

void MainWindow::updateUI() {
    labelBasket1Red->setText(QString::number(basket1.GetRed()));
    labelBasket1Blue->setText(QString::number(basket1.GetBlue()));

    double prob1Red = basket1.getRedProbability() * 100;
    double prob1Blue = basket1.getBlueProbability() * 100;
    labelBasket1ProbRed->setText(QString("%1%").arg(prob1Red, 0, 'f', 2));
    labelBasket1ProbBlue->setText(QString("%1%").arg(prob1Blue, 0, 'f', 2));

    labelBasket2Red->setText(QString::number(basket2.GetRed()));
    labelBasket2Blue->setText(QString::number(basket2.GetBlue()));

    double prob2Red = basket2.getRedProbability() * 100;
    double prob2Blue = basket2.getBlueProbability() * 100;
    labelBasket2ProbRed->setText(QString("%1%").arg(prob2Red, 0, 'f', 2));
    labelBasket2ProbBlue->setText(QString("%1%").arg(prob2Blue, 0, 'f', 2));

    double probRR = 0.0, probBB = 0.0, probRB = 0.0;

    bool canTwoFrom1 = basket1.total() >= 2;
    bool canTwoFrom2 = basket2.total() >= 2;
    bool canOneFromEach = basket1.total() >= 1 && basket2.total() >= 1;

    std::vector<int> options;
    if (canTwoFrom1) options.push_back(1);
    if (canTwoFrom2) options.push_back(2);
    if (canOneFromEach) options.push_back(3);

    int optionCount = options.size();
    if (optionCount == 0) {
        probRR = probBB = probRB = 0.0;
    } else {
        double variantProb = 1.0 / optionCount;
        for (int option : options) {
            double rr = 0.0, bb = 0.0, rb = 0.0;
            if (option == 1) {
                int r = basket1.GetRed();
                int b = basket1.GetBlue();
                int t = r + b;
                if (t >= 2) {
                    rr = (r * (r - 1.0)) / (t * (t - 1.0));
                    bb = (b * (b - 1.0)) / (t * (t - 1.0));
                    rb = (2.0 * r * b) / (t * (t - 1.0));
                }
            } else if (option == 2) {
                int r = basket2.GetRed();
                int b = basket2.GetBlue();
                int t = r + b;
                if (t >= 2) {
                    rr = (r * (r - 1.0)) / (t * (t - 1.0));
                    bb = (b * (b - 1.0)) / (t * (t - 1.0));
                    rb = (2.0 * r * b) / (t * (t - 1.0));
                }
            } else if (option == 3) {
                int r1 = basket1.GetRed();
                int b1 = basket1.GetBlue();
                int t1 = r1 + b1;
                int r2 = basket2.GetRed();
                int b2 = basket2.GetBlue();
                int t2 = r2 + b2;
                if (t1 >= 1 && t2 >= 1) {
                    double p1r = static_cast<double>(r1) / t1;
                    double p1b = static_cast<double>(b1) / t1;
                    double p2r = static_cast<double>(r2) / t2;
                    double p2b = static_cast<double>(b2) / t2;
                    rr = p1r * p2r;
                    bb = p1b * p2b;
                    rb = p1r * p2b + p1b * p2r;
                }
            }
            probRR += rr * variantProb;
            probBB += bb * variantProb;
            probRB += rb * variantProb;
        }
    }

    labelTwoRedProb->setText(QString("%1%").arg(probRR * 100, 0, 'f', 2));
    labelTwoBlueProb->setText(QString("%1%").arg(probBB * 100, 0, 'f', 2));
    labelMixedProb->setText(QString("%1%").arg(probRB * 100, 0, 'f', 2));

    // Обновляем визуализацию если окно открыто
    if (visWindow) {
        visWindow->updateVisualization(basket1, basket2);
    }
}

void MainWindow::applyValues() {
    basket1 = Basket(spinRed1->value(), spinBlue1->value());
    basket2 = Basket(spinRed2->value(), spinBlue2->value());
    logAction("Настройки корзин обновлены через интерфейс");
    updateUI();
}

void MainWindow::showVisualization() {
    if (!visWindow) {
        visWindow = new VisualizationWindow(basket1, basket2, this);
    }
    visWindow->updateVisualization(basket1, basket2);
    visWindow->show();
}

void MainWindow::onMove1to2() {
    if (basket1.total() == 0) {
        QMessageBox::information(this, "Внимание", "Корзина 1 пуста");
        return;
    }

    bool isRed = QRandomGenerator::global()->bounded(basket1.total()) < basket1.GetRed();
    QString color = isRed ? "красный" : "синий";
    if (isRed) {
        basket1.removeRed(1);
        basket2.addRed(1);
    } else {
        basket1.removeBlue(1);
        basket2.addBlue(1);
    }
    logAction(QString("Перемещен %1 шар из корзины 1 в корзину 2").arg(color));
    updateUI();
}

void MainWindow::onMove2to1() {
    if (basket2.total() == 0) {
        QMessageBox::information(this, "Внимание", "Корзина 2 пуста");
        return;
    }

    bool isRed = QRandomGenerator::global()->bounded(basket2.total()) < basket2.GetRed();
    QString color = isRed ? "красный" : "синий";
    if (isRed) {
        basket2.removeRed(1);
        basket1.addRed(1);
    } else {
        basket2.removeBlue(1);
        basket1.addBlue(1);
    }
    logAction(QString("Перемещен %1 шар из корзины 2 в корзину 1").arg(color));
    updateUI();
}

void MainWindow::logAction(const QString &message) {
    QString timestamp = QDateTime::currentDateTime().toString("HH:mm:ss");
    QString fullMessage = QString("[%1] %2").arg(timestamp, message);

    while (logTextEdit->document()->blockCount() > 3) {
        QTextCursor cursor(logTextEdit->document());
        cursor.movePosition(QTextCursor::Start);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
        cursor.deleteChar();
    }

    logTextEdit->appendPlainText(fullMessage);
}

void MainWindow::onDrawTwo() {
    bool canTwoFrom1 = basket1.total() >= 2;
    bool canTwoFrom2 = basket2.total() >= 2;
    bool canOneFromEach = basket1.total() >= 1 && basket2.total() >= 1;

    std::vector<int> options;
    if (canTwoFrom1) options.push_back(1);
    if (canTwoFrom2) options.push_back(2);
    if (canOneFromEach) options.push_back(3);

    if (options.empty()) {
        QMessageBox::information(this, "Внимание", "Недостаточно шаров для извлечения");
        return;
    }

    int selectedOption = options[QRandomGenerator::global()->bounded(options.size())];

    if (selectedOption == 1) {
        if (basket1.total() < 2) return;
        int r = basket1.GetRed();
        int b = basket1.GetBlue();
        int total = r + b;

        bool firstRed = QRandomGenerator::global()->bounded(total) < r;
        int redCount = firstRed ? 1 : 0;
        int blueCount = firstRed ? 0 : 1;

        int remainingTotal = total - 1;
        int remainingRed = r - redCount;
        int remainingBlue = b - blueCount;
        bool secondRed = QRandomGenerator::global()->bounded(remainingTotal) < remainingRed;
        QString firstColor = firstRed ? "красный" : "синий";
        QString secondColor = secondRed ? "красный" : "синий";
        logAction(QString("Извлечено из корзины 1: %1 и %2 шары").arg(firstColor, secondColor));
        redCount += secondRed ? 1 : 0;
        blueCount += secondRed ? 0 : 1;

        basket1.removeRed(redCount);
        basket1.removeBlue(blueCount);
    } else if (selectedOption == 2) {
        if (basket2.total() < 2) return;
        int r = basket2.GetRed();
        int b = basket2.GetBlue();
        int total = r + b;

        bool firstRed = QRandomGenerator::global()->bounded(total) < r;
        int redCount = firstRed ? 1 : 0;
        int blueCount = firstRed ? 0 : 1;

        int remainingTotal = total - 1;
        int remainingRed = r - redCount;
        int remainingBlue = b - blueCount;
        bool secondRed = QRandomGenerator::global()->bounded(remainingTotal) < remainingRed;
        QString firstColor = firstRed ? "красный" : "синий";
        QString secondColor = secondRed ? "красный" : "синий";

        logAction(QString("Извлечено из корзины 2: %1 и %2 шары").arg(firstColor, secondColor));

        redCount += secondRed ? 1 : 0;
        blueCount += secondRed ? 0 : 1;

        basket2.removeRed(redCount);
        basket2.removeBlue(blueCount);
    } else if (selectedOption == 3) {
        if (basket1.total() < 1 || basket2.total() < 1) return;
        bool red1 = QRandomGenerator::global()->bounded(basket1.total()) < basket1.GetRed();
        basket1.removeRed(red1 ? 1 : 0);
        basket1.removeBlue(!red1 ? 1 : 0);

        bool red2 = QRandomGenerator::global()->bounded(basket2.total()) < basket2.GetRed();
        QString color1 = red1 ? "красный" : "синий";
        QString color2 = red2 ? "красный" : "синий";

        logAction(QString("Извлечено: %1 из корзины 1 и %2 из корзины 2").arg(color1, color2));
        basket2.removeRed(red2 ? 1 : 0);
        basket2.removeBlue(!red2 ? 1 : 0);
    }
    updateUI();
}
