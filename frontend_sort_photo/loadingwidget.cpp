#include "loadingwidget.h"

#include <QTimer>
#include <QMovie>
#include <QDesktopServices>

LoadingWidget::LoadingWidget(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Загрузка...");
    setFixedSize(800, 500);

    // Настройка цвета и градиента фона
    QPalette palette;
    QLinearGradient gradient(0, 0, 0, 500); // Заменяем height() на фиксированную высоту
    gradient.setColorAt(0.0, QColor(255, 203, 243));
    gradient.setColorAt(1.0, QColor(128, 0, 128));
    palette.setBrush(QPalette::Window, QBrush(gradient));
    setAutoFillBackground(true);
    setPalette(palette);


    // Создание Layout
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Создаем метки
    loadingLabel = new QLabel("Идёт сортировка фотографий...", this);
    loadingLabel->setStyleSheet("font-size: 24px; color: rgb(128, 0, 128);; font-weight: bold;");
    loadingLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(loadingLabel);

    // Инициализируем статус метку
    statusLabel = new QLabel("Статус: Начало обработки", this);
    statusLabel->setStyleSheet("font-size: 24px; color: rgb(128, 0, 128);; font-weight: bold;");
    statusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(statusLabel);

    // Создаем вращающийся индикатор загрузки
    spinnerLabel = new QLabel(this);
    spinnerLabel->setAlignment(Qt::AlignCenter);

    // Загружаем GIF-анимацию для спиннера
    QMovie *spinnerMovie = new QMovie(":/images/cat.gif", QByteArray(), this);
    spinnerLabel->setMovie(spinnerMovie);
    spinnerMovie->start();
    layout->addWidget(spinnerLabel);
    if (!spinnerMovie->isValid()) {
        qWarning("GIF file is not valid or failed to load.");
    }


    // Кнопка продолжения
    continueButton = new QPushButton("Перейти к результатам", this);
    continueButton->setStyleSheet("background-color: #008CBA; color: white; font-size: 16px; padding: 10px;");
    continueButton->setEnabled(true);  // Отключаем кнопку, пока прогресс не завершится
    continueButton->hide();  // Скрываем кнопку при инициализации
    layout->addWidget(continueButton);

    // Таймер для отслеживания времени загрузки
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this, timer]() {
        static int elapsed = 0;
        elapsed += 50; // Увеличиваем время на 50 мс
        if (elapsed < 2000) { // Например, процесс завершится за 5 секунд
            statusLabel->setText("Статус: Загружается... (" + QString::number(elapsed / 1000.0, 'f', 1) + " сек)");


        } else {
            continueButton->setEnabled(true);
            spinnerLabel->hide();  // Скрываем GIF-анимацию
            loadingLabel->hide();
            statusLabel->setText("Статус: Сортировка завершена");
            statusLabel->setStyleSheet("font-size: 24px; color: rgb(128, 0, 128);; font-weight: bold;");
            statusLabel->setAlignment(Qt::AlignCenter);
            continueButton->show();  // Показываем кнопку продолжения
            timer->stop();  // Останавливаем таймер
        }
    });
    timer->start(50); // Запуск таймера с интервалом 50 мс

    // Подключаем кнопку
    connect(continueButton, &QPushButton::clicked, this, &LoadingWidget::onContinueButtonClicked);
    setLayout(layout);
}

void LoadingWidget::setTargetFolderPath(const QString &path) {
    targetFolderPath = path;
}

void LoadingWidget::onContinueButtonClicked() {
    if (!targetFolderPath.isEmpty()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(targetFolderPath));
    }
    emit nextWindowRequested();  // Испускаем сигнал для перехода к следующему окну
}
