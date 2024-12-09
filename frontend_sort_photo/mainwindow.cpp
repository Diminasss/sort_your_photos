#include "mainwindow.h"
#include "sortingwidget.h"

#include <QPalette>
#include <QLinearGradient>
#include <QFileDialog>
#include <QDebug> // Для отладочного вывода

MainWindow::MainWindow (QWidget *parent) : QWidget(parent),  sortingWidget(nullptr) {
    // Установка заголовка окна
    setWindowTitle("FileSend");


    // Настройка цвета и градиента фона
    QPalette palette;
    QLinearGradient gradient(0, 0, 0, 500); // Заменяем height() на фиксированную высоту
    gradient.setColorAt(0.0, QColor(255, 203, 243));
    gradient.setColorAt(1.0, QColor(128, 0, 128));
    palette.setBrush(QPalette::Window, QBrush(gradient));
    setAutoFillBackground(true);
    setPalette(palette);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Заголовок
    QLabel *titleLabel = new QLabel("Выбор папки для сортировки", this);
    titleLabel->setStyleSheet("font-size: 24px; color: white; font-weight: bold;");
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *subtitleLabel = new QLabel("Сортировка может занять некоторое время...", this);
    subtitleLabel->setStyleSheet("font-size: 16px; color: white;");
    subtitleLabel->setAlignment(Qt::AlignCenter);

    QLabel *pathLabel1 = new QLabel("Укажите путь к неотсортированной папке с фото:", this);
    pathLabel1->setStyleSheet("font-size: 14px; color: black;");

    pathEdit1 = new QLineEdit(this);
    pathEdit1->setPlaceholderText("Выберите путь...");
    pathEdit1->setStyleSheet("background-color: white; padding: 5px; color: purple");

    QPushButton *browseButton1 = new QPushButton("Обзор...", this);
    browseButton1->setStyleSheet("background-color: #cc66cc; color: white; padding: 5px;");
    connect(browseButton1, &QPushButton::clicked, this, &MainWindow::browseForFolder);

    QHBoxLayout *pathLayout1 = new QHBoxLayout;
    pathLayout1->addWidget(pathEdit1);
    pathLayout1->addWidget(browseButton1);

    QLabel *pathLabel2 = new QLabel("Укажите путь к папке, в которую будут размещены отсортированные фото:", this);
    pathLabel2->setStyleSheet("font-size: 14px; color: black;");

    QLineEdit *pathEdit2 = new QLineEdit(this);
    pathEdit2->setPlaceholderText("Выберите путь...");
    pathEdit2->setStyleSheet("background-color: white; padding: 5px; color: blue");

    QHBoxLayout *pathLayout2 = new QHBoxLayout;
    pathLayout2->addWidget(pathEdit2);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addWidget(pathLabel1);
    mainLayout->addLayout(pathLayout1);
    mainLayout->addWidget(pathLabel2);
    mainLayout->addLayout(pathLayout2);

    sortByNameRadio = new QRadioButton("Сортировка фото по названию телефона", this);
    sortByDateRadio = new QRadioButton("Сортировка фото по датам", this);
    sortByNameRadio->setStyleSheet("font-size: 14px; color: black;");
    sortByDateRadio->setStyleSheet("font-size: 14px; color: black;");

    sortGroup = new QButtonGroup(this);
    sortGroup->addButton(sortByNameRadio);
    sortGroup->addButton(sortByDateRadio);
    sortByNameRadio->setChecked(true);

    mainLayout->addWidget(sortByNameRadio);
    mainLayout->addWidget(sortByDateRadio);

    nextButton = new QPushButton("Сортировать", this);
    nextButton->setStyleSheet("background-color: #008CBA; color: white; font-size: 16px; padding: 10px;");
    //connect(nextButton, SIGNAL(clicked()), this, SLOT(openSortingWindow()));
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::openSortingWindow);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(nextButton);
    mainLayout->addLayout(buttonLayout);

    setFixedSize(800, 500);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // ui->setupUi(this);
    // // Инициализируем второе окно
    // sortingWidget = new SortingWidget();
    // // подключаем к слоту запуска главного окна по кнопке во втором окне
    // connect(nextButton, &SortingWidget::firstWindow, this, &MainWindow::show);
}


void MainWindow ::browseForFolder() {
    QString folderPath = QFileDialog::getExistingDirectory(this, "Выберите папку", "");
    if (!folderPath.isEmpty()) {
        pathEdit1->setText(folderPath);
    }
}

void MainWindow::openSortingWindow() {
    if (!sortingWidget) {
        sortingWidget = new SortingWidget();
        qDebug() << "SortingWidget created.";
    }
    sortingWidget->show();
    qDebug() << "SortingWidget shown.";
    this->hide();
    qDebug() << "Main window hidden.";

    // this->hide();
    // sortingWidget = new SortingWidget();
    // sortingWidget->show();

}
