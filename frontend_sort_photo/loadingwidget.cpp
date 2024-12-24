#include "loadingwidget.h"

// Окно загрузки
LoadingWidget::LoadingWidget(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Sort Your Photos - Идёт сортировка файлов, не выключайте программу...");
    setFixedSize(800, 500);
    QIcon icon("app_icon.ico");
    setWindowIcon(icon);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        elapsedTime += 50;
        statusLabel->setText("Статус: сортировка (" + QString::number(elapsedTime / 1000.0, 'f', 1) + " сек)");
        });
    timer->start(50);

   
    QPalette palette;
    QLinearGradient gradient(0, 0, 0, 500); 
    gradient.setColorAt(0.0, QColor(255, 203, 243));
    gradient.setColorAt(1.0, QColor(128, 0, 128));
    palette.setBrush(QPalette::Window, QBrush(gradient));
    setAutoFillBackground(true);
    setPalette(palette);

    QVBoxLayout *layout = new QVBoxLayout(this);

    loadingLabel = new QLabel("Идёт сортировка фотографий...", this);
    loadingLabel->setStyleSheet("font-size: 24px; color: rgb(128, 0, 128);; font-weight: bold;");
    loadingLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(loadingLabel);

    statusLabel = new QLabel("Статус: Начало обработки", this);
    statusLabel->setStyleSheet("font-size: 24px; color: rgb(128, 0, 128);; font-weight: bold;");
    statusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(statusLabel);

    spinnerLabel = new QLabel(this);
    spinnerLabel->setAlignment(Qt::AlignCenter);

    QMovie *spinnerMovie = new QMovie(":/images/cat.gif", QByteArray(), this);
    spinnerLabel->setMovie(spinnerMovie);
    spinnerMovie->start();
    layout->addWidget(spinnerLabel);
    if (!spinnerMovie->isValid()) {
        qWarning("GIF file is not valid or failed to load.");
    }

    continueButton = new QPushButton("Перейти к результатам", this);
    continueButton->setStyleSheet("background-color: #008CBA; color: white; font-size: 16px; padding: 10px;");
    continueButton->setEnabled(true);  
    continueButton->hide();  
    layout->addWidget(continueButton);

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
    emit nextWindowRequested();  
}

void LoadingWidget::sortingFinished() {
    writeLog("Метод SortingFinished");
    if (timer) {
        if (timer->isActive()) {
            timer->stop();
            writeLog("Таймер остановлен.");
        }
        delete timer;
        timer = nullptr;
        writeLog("Таймер удалён.");
    }
    else {
        writeLog("Таймер не существует.");
    }
    setWindowTitle("Sort Your Photos - Сортировка завершена");
    continueButton->setEnabled(true);
    
    spinnerLabel->hide();  
    loadingLabel->hide();
    statusLabel->setText("Статус: cортировка завершена за " + QString::number(elapsedTime / 1000.0) + " секунд");
    statusLabel->setStyleSheet("font-size: 24px; color: rgb(128, 0, 128);; font-weight: bold;");
    statusLabel->setAlignment(Qt::AlignCenter);
    continueButton->show();  
}