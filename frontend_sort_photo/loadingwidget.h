// loadingwidget.h

#pragma once

#ifndef LOADINGWIDGET_H
#define LOADINGWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QLabel>

class LoadingWidget : public QWidget {
    Q_OBJECT  // Убедитесь, что макрос Q_OBJECT присутствует

public:
    explicit LoadingWidget(QWidget *parent = nullptr);

signals:
    void nextWindowRequested();  // Сигнал без параметров  // Сигнал для перехода к следующему окну


private slots:
    void onContinueButtonClicked(); // Слот для обработки нажатия кнопки

private:
    QProgressBar *progressBar;  // Прогресс-бар
    QPushButton *continueButton;  // Кнопка для продолжения
    QLabel *loadingLabel;  // Подсказка
    QLabel *statusLabel;
};

#endif // LOADINGWIDGET_H
