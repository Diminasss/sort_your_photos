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
#include <QProgressBar>  // Для прогресс-бара

#include "sortingwidget.h"
#include "loadingwidget.h"

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
    //void browseForFolder(); // Слот для обработки выбора папки
    void browseForSourceFolder();
    void browseForTargetFolder();
    void openSortingWindow(); // Слот для открытия нового окна
    void openLoadingWindow();  // Новый слот для открытия окна с загрузкой


private:
    QLineEdit *pathEdit1; // Указатель на поле pathEdit1
    QLineEdit *pathEdit2;  // Объявление pathEdit2 как члена класса// Указатель на поле pathEdit2, если потребуется
    QRadioButton *sortByNameRadio; // Радиокнопка для сортировки по названию телефона
    QRadioButton *sortByDateRadio; // Радиокнопка для сортировки по дате
    QButtonGroup *sortGroup;       // Группа радиокнопок для выбора метода сортировки

    QPushButton *nextButton;  // Кнопка "Далее"
    SortingWidget *sortingWidget; // Указатель на новое окно

    LoadingWidget *loadingWidget;  // Указатель на окно загрузки
    QPushButton *continueButton;  // Кнопка для продолжения после загрузки

};


#endif // SORT_FRONT_H
