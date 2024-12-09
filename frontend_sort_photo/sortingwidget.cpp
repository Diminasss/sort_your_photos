#include "sortingwidget.h"

SortingWidget::SortingWidget(QWidget *parent) : QWidget(parent) {

    setWindowTitle("Sorting Window");
    setFixedSize(800, 500);
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("Сортировка файлов...", this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size: 24px; color: black; background-color: purple;");
    layout->addWidget(label);
    show();

}



    // Устанавливаем компоновщик
    //setLayout(layout);
