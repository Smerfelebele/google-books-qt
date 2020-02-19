#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "engine.h"
#include <QTableWidget>
#include <QCheckBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget //public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);


    QGridLayout* grid;

    QCheckBox* authorBox;
    QCheckBox* titleBox;
    QCheckBox* publisherBox;
    QCheckBox* subjectBox;
    QPushButton* searchButton;

    QLineEdit* authorTextField;
    QLineEdit* titleTextField;
    QLineEdit* publisherTextField;
    QLineEdit* subjectTextField;
    QLineEdit* searchTextField;

    QNetworkAccessManager *networkAccessManager;

    ~MainWindow();
signals:
    void collectedAllData();


private slots:
    void handleSearchButton();
    void replyFinished(QNetworkReply* );
    void createTableGrid();
    void updateTable();


private:

    //void updateTable();

    QVector<QString> titleVec;
    QVector<QStringList> authorsVec;
    //QStringList authors;
    QVector<QString> languageVec;
    QVector<QString> publisherVec;
    QVector<double> averageRatingVec;
    QVector<QString> publishedDateVec;

    QTableWidget* m_pTableWidget;
    QStringList m_TableHeader;

    QJsonArray insideItemsValue;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
