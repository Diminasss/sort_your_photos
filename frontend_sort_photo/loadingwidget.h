#pragma once

#ifndef LOADINGWIDGET_H
#define LOADINGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>
#include <QPalette>
#include <QLinearGradient>
#include <QMovie>
#include <QDesktopServices>
#include <QUrl>
#include <QIcon>
#include <QString>
#include <QMessageBox>

#include "../ProgramFiles/CommonFunctions/writeLog.h"
#define writeLog(msg) writeLog(msg, __FILE__, __LINE__)

class LoadingWidget : public QWidget {
    Q_OBJECT 

public:
    explicit LoadingWidget(QWidget *parent = nullptr);
    void setTargetFolderPath(const QString &path);  
    void sortingFinished();

signals:
    void nextWindowRequested();  


private slots:
    void onContinueButtonClicked(); 

private:
    QPushButton *continueButton;  
    QLabel *loadingLabel;  
    QLabel *statusLabel;
    QLabel *spinnerLabel;
    QString targetFolderPath;
    QTimer *timer;
    int elapsedTime = 0;
};

#endif 