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
    void setTargetFolderPath(const QString &path);  // Метод для установки пути к папке

signals:
    void nextWindowRequested();  // Сигнал для перехода к следующему окну


private slots:
    void onContinueButtonClicked(); // Слот для обработки нажатия кнопки

private:
    //QProgressBar *progressBar;  // Прогресс-бар
    QPushButton *continueButton;  // Кнопка для продолжения
    QLabel *loadingLabel;  // Подсказка
    QLabel *statusLabel;
    QLabel *spinnerLabel;
    QString targetFolderPath;
};

#endif // LOADINGWIDGET_H
