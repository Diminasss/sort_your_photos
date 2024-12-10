#include "sortingwidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

SortingWidget::SortingWidget(QWidget *parent) : QWidget(parent) {

    setFixedSize(800, 500);

    QPalette palette;
    QLinearGradient gradient(0, 0, 0, 500); // Заменяем height() на фиксированную высоту
    gradient.setColorAt(0.0, QColor(255, 203, 243));
    gradient.setColorAt(1.0, QColor(128, 0, 128));
    palette.setBrush(QPalette::Window, QBrush(gradient));
    setAutoFillBackground(true);
    setPalette(palette);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Сортировка фото", this);
    layout->addWidget(titleLabel);

    QPushButton *startButton = new QPushButton("Начать сортировку", this);
    connect(startButton, &QPushButton::clicked, this, &SortingWidget::startSorting);
    layout->addWidget(startButton);

    setLayout(layout);
    setWindowTitle("Sorting");

    // setWindowTitle("Sorting Window");
    // setFixedSize(800, 500);
    // QVBoxLayout *layout = new QVBoxLayout(this);
    // QLabel *label = new QLabel("Сортировка файлов...", this);
    // label->setAlignment(Qt::AlignCenter);
    // label->setStyleSheet("font-size: 24px; color: black;");
    // layout->addWidget(label);
    // show();
}

void SortingWidget::startSorting() {
    // Реализация метода
}




    // Устанавливаем компоновщик
    //setLayout(layout);
