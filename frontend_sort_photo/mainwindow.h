#pragma once
#ifndef SORT_FRONT_H
#define SORT_FRONT_H

#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QFileDialog>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>
#include <QMetaObject>
#include <QPalette>
#include <QLinearGradient>
#include <QIcon>


#include "loadingwidget.h"

#include "../ProgramFiles/SortPhotosAlgorithms/SortByGadgetName.h"
#include "../ProgramFiles/SortPhotosAlgorithms/SortByYear.h"

#include "../ProgramFiles/CommonFunctions/writeLog.h"
#define writeLog(msg) writeLog(msg, __FILE__, __LINE__)


class MainWindow : public QWidget {
    Q_OBJECT


public:
    explicit MainWindow (QWidget *parent = nullptr);


private slots:
    void browseForSourceFolder();
    void browseForTargetFolder();
    void openLoadingWindow();  


private:
    QLineEdit *pathEdit1; 
    QLineEdit *pathEdit2;  
    QRadioButton *sortByNameRadio; 
    QRadioButton *sortByDateRadio; 
    QButtonGroup *sortGroup;       

    QPushButton *nextButton;  

    LoadingWidget *loadingWidget; 
    QPushButton *continueButton; 

};
#endif 