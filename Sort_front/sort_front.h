#ifndef SORT_FRONT_H
#define SORT_FRONT_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

// QT_BEGIN_NAMESPACE
// namespace Ui {
// class Sort_front;
// }
// QT_END_NAMESPACE

// class Sort_front : public QMainWindow
// {
//     Q_OBJECT

// public:
//     Sort_front(QWidget *parent = nullptr);
//     ~Sort_front();

// private:
//     Ui::Sort_front *ui;
// };
// #endif // SORT_FRONT_H

class FileSendWidget : public QWidget {
    Q_OBJECT

public:
    explicit FileSendWidget(QWidget *parent = nullptr);
    //QLineEdit *pathEdit1;

private slots:
    void browseForFolder(); // Слот для обработки выбора папки

private:
    QLineEdit *pathEdit1;
};

#endif // SORT_FRONT_H
