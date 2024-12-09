#pragma once
#ifndef SORT_FRONT_H
#define SORT_FRONT_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>

#include "sortingwidget.h"

// namespace Ui {
// class MainWindow;
// }

class MainWindow : public QWidget {
    Q_OBJECT


public:
    explicit MainWindow (QWidget *parent = nullptr);
    //~FileSendWidget() override; // Деструктор
    //QLineEdit *pathEdit1;


private slots:
    void browseForFolder(); // Слот для обработки выбора папки

    void openSortingWindow(); // Слот для открытия нового окна


private:
    QLineEdit *pathEdit1; // Указатель на поле pathEdit1
    QLineEdit *pathEdit2; // Указатель на поле pathEdit2, если потребуется
    QRadioButton *sortByNameRadio; // Радиокнопка для сортировки по названию телефона
    QRadioButton *sortByDateRadio; // Радиокнопка для сортировки по дате
    QButtonGroup *sortGroup;       // Группа радиокнопок для выбора метода сортировки

    QPushButton *nextButton;  // Кнопка "Далее"
    SortingWidget *sortingWidget; // Указатель на новое окно

    //Ui::MainWindow *ui;

};


#endif // SORT_FRONT_H

// #ifndef MAINWINDOW_H
// #define MAINWINDOW_H

// #include <QMainWindow>
// #include <sortingwidget.h>

// namespace Ui {
// class MainWindow;
// }

// class MainWindow : public QMainWindow
// {
//     Q_OBJECT

// public:
//     explicit MainWindow(QWidget *parent = 0);
//     ~MainWindow();

// private slots:
//     // Слоты от кнопок главного окна
//     void on_pushButton_clicked();
//     void on_pushButton_2_clicked();

// private:
//     Ui::MainWindow *ui;
//     // второе и третье окна
//     AnotherWindow *nextButton;
//     AnotherWindow *thirdWindow;
// };

// #endif // MAINWINDOW_H
