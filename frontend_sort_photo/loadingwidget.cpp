// loadingwidget.cpp
#include "loadingwidget.h"
#include <QTimer>

LoadingWidget::LoadingWidget(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Загрузка...");
    setFixedSize(800, 500);

    // Создание Layout
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Создаем метки
    loadingLabel = new QLabel("Идёт сортировка фотографий...", this);
    loadingLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(loadingLabel);

    // Инициализируем статус метку
    statusLabel = new QLabel("Статус: Начало обработки", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(statusLabel);

    // Прогресс-бар
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);  // Диапазон от 0 до 100%
    progressBar->setValue(0);  // Начальное значение
    layout->addWidget(progressBar);

    // Кнопка продолжения
    continueButton = new QPushButton("Перейти к результатам", this);
    continueButton->setEnabled(false);  // Отключаем кнопку, пока прогресс не завершится
    layout->addWidget(continueButton);

    // Имитация процесса загрузки
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this, timer]() {
        int value = progressBar->value();
        if (value < 100) {
            progressBar->setValue(value + 1);
            // Обновление статуса
            statusLabel->setText("Статус: Загружается... " + QString::number(value) + "%");
        } else {
            continueButton->setEnabled(true);  // Включаем кнопку, когда прогресс завершён
            statusLabel->setText("Статус: Завершено");  // Обновление статуса
            timer->stop();  // Останавливаем таймер
        }
    });
    timer->start(50);  // Запуск таймера с интервалом 50 мс

    // Подключаем кнопку
    connect(continueButton, &QPushButton::clicked, this, &LoadingWidget::onContinueButtonClicked);

    setLayout(layout);
}

void LoadingWidget::onContinueButtonClicked() {
    emit nextWindowRequested();  // Испускаем сигнал для перехода к следующему окну
}
