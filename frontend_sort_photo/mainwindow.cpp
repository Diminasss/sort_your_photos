#include "mainwindow.h"

#include <QPalette>
#include <QLinearGradient>
#include <QFileDialog>

FileSendWidget::FileSendWidget(QWidget *parent) : QWidget(parent) {
    // Set window title
    setWindowTitle("FileSend");

    // Background color and gradient setup
    QPalette palette;
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0, QColor(255, 203, 243));
    gradient.setColorAt(1.0, QColor(128, 0, 128));
    palette.setBrush(QPalette::Window, QBrush(gradient));
    setAutoFillBackground(true);
    setPalette(palette);

    // Main layout setup
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Title label
    QLabel *titleLabel = new QLabel("Выбор папки для сортировки", this);
    titleLabel->setStyleSheet("font-size: 24px; color: white; font-weight: bold;");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Subtitle label
    QLabel *subtitleLabel = new QLabel("...?", this);
    subtitleLabel->setStyleSheet("font-size: 16px; color: white;");
    subtitleLabel->setAlignment(Qt::AlignCenter);

    // Path label 1
    QLabel *pathLabel1 = new QLabel("Укажите путь к неотсортированной папке с фото:", this);
    pathLabel1->setStyleSheet("font-size: 14px; color: black;");

    QLineEdit *pathEdit1 = new QLineEdit(this);
    pathEdit1->setPlaceholderText("Выберите путь...");
    // pathEdit1->setText("C:\\Users\\nensi\\Documents\\SUA\\Course3\\sort_your_photos\\sort_front");
    pathEdit1->setStyleSheet("background-color: white; padding: 5px; color: purple");

    QPushButton *browseButton1 = new QPushButton("Обзор...", this);
    browseButton1->setStyleSheet("background-color: #cc66cc; color: white; padding: 5px;");
    connect(browseButton1, &QPushButton::clicked, this, &FileSendWidget::browseForFolder);

    // Layout for the first path entry
    QHBoxLayout *pathLayout1 = new QHBoxLayout;
    pathLayout1->addWidget(pathEdit1);
    pathLayout1->addWidget(browseButton1);


    // Радиокнопки для выбора способа сортировки
    sortByNameRadio = new QRadioButton("Сортировка фото по названию телефона", this);
    sortByDateRadio = new QRadioButton("Сортировка фото по датам", this);
    sortByNameRadio->setStyleSheet("font-size: 14px; color: black;");
    sortByDateRadio->setStyleSheet("font-size: 14px; color: black;");

    // Группа радиокнопок для сортировки
    sortGroup = new QButtonGroup(this);
    sortGroup->addButton(sortByNameRadio);
    sortGroup->addButton(sortByDateRadio);

    // Установка первой радиокнопки как выбранной по умолчанию
    sortByNameRadio->setChecked(true);



    // Path label 2
    QLabel *pathLabel2 = new QLabel("Укажите путь к папке, в которую будут размещены отсортированные фото:", this);
    pathLabel2->setStyleSheet("font-size: 14px; color: black;");
    QLineEdit *pathEdit2 = new QLineEdit(this);
    pathEdit2->setPlaceholderText("Выберите путь...");
    pathEdit2->setStyleSheet("background-color: white; padding: 5px; color: blue");

    // Layout for the second path entry
    QHBoxLayout *pathLayout2 = new QHBoxLayout;
    pathLayout2->addWidget(pathEdit2);

    // Add widgets to the main layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addWidget(pathLabel1);
    mainLayout->addLayout(pathLayout1);
    mainLayout->addWidget(pathLabel2);
    mainLayout->addLayout(pathLayout2);

    // Добавляем радиокнопки в основной компоновщик
    mainLayout->addWidget(sortByNameRadio);
    mainLayout->addWidget(sortByDateRadio);

    // Set margins and spacing for the main layout to match the design
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Set fixed size for the widget to approximate the screenshot dimensions
    setFixedSize(800, 500);
}

// Слот для открытия диалога выбора папки и отображения выбранного пути
void FileSendWidget::browseForFolder() {
    QString folderPath = QFileDialog::getExistingDirectory(this, "Выберите папку", "");

    if (!folderPath.isEmpty()) {
        pathEdit1->setText(folderPath);  // Устанавливаем выбранный путь в pathEdit1
    }
}
// Sort_front::Sort_front(QWidget *parent)
//     : QMainWindow(parent)
//     , ui(new Ui::Sort_front)
// {
//     ui->setupUi(this);
// }

// Sort_front::~Sort_front()
// {
//     delete ui;
// }
