#ifndef SORTINGWIDGET_H
#define SORTINGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class SortingWidget : public QWidget {
    Q_OBJECT

public:
    explicit SortingWidget(QWidget *parent = nullptr);
    void startSorting();

// private:
//     void startSorting();
};

#endif // SORTINGWIDGET_H
