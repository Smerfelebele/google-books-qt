#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QHeaderView>
#include <QMessageBox>
#include <QDesktopWidget>
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent) //QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    //Engine engine;
    grid = new QGridLayout;
    networkAccessManager = new QNetworkAccessManager(this);
    //QPushButton* searchButton = new QPushButton("Search");
    //searchTextField = new QLineEdit(searchButton);

    //horizontalLayout->addWidget(searchTextField);
    //horizontalLayout->addWidget(searchButton);

    QHBoxLayout *authorLayout = new QHBoxLayout;
    authorBox = new QCheckBox("Author:");
    authorTextField = new QLineEdit(authorBox);
    authorLayout->addWidget(authorBox);
    authorLayout->addWidget(authorTextField);


    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleBox = new QCheckBox("Title:");
    titleTextField = new QLineEdit(titleBox);
    titleLayout->addWidget(titleBox);
    titleLayout->addWidget(titleTextField);

    QHBoxLayout *publisherLayout = new QHBoxLayout;
    publisherBox = new QCheckBox("Publisher:");
    publisherTextField = new QLineEdit(publisherBox);
    publisherLayout->addWidget(publisherBox);
    publisherLayout->addWidget(publisherTextField);

    QHBoxLayout *subjectLayout = new QHBoxLayout;
    subjectBox = new QCheckBox("Subject:");
    subjectTextField = new QLineEdit(subjectBox);
    subjectLayout->addWidget(subjectBox);
    subjectLayout->addWidget(subjectTextField);

    QHBoxLayout *searchButtonLayout = new QHBoxLayout;
    QPushButton* searchButton = new QPushButton("Search");
    searchButtonLayout->addWidget(searchButton);

    QHBoxLayout *searchTextLayout = new QHBoxLayout;
    searchTextField = new QLineEdit();
    searchTextLayout->addWidget(searchTextField);

    QVBoxLayout *searchAreaLayout = new QVBoxLayout;
    searchAreaLayout->addLayout(authorLayout);
    searchAreaLayout->addLayout(titleLayout);
    searchAreaLayout->addLayout(publisherLayout);
    searchAreaLayout->addLayout(subjectLayout);
    searchAreaLayout->addLayout(searchTextLayout);
    searchAreaLayout->addLayout(searchButtonLayout);

    grid->addLayout(searchAreaLayout,0,0);
    createTableGrid();

    setLayout(grid);
    //setFixedSize(400,200);
    //setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    adjustSize();
    //setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));
    setFixedSize(1050,600);
    connect(searchButton, SIGNAL (clicked()),this, SLOT (handleSearchButton()));
    connect(networkAccessManager, SIGNAL(finished(QNetworkReply *)),this, SLOT(replyFinished(QNetworkReply* )));
    connect(this, SIGNAL(collectedAllData()), this, SLOT(updateTable()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleSearchButton()
{
    qDebug() << "Clicked search";
    //QString str = QString("https://www.googleapis.com/books/v1/volumes?q=%1").arg(searchTextField->text());
    QString authorValue="", titleValue="",publisherValue="", subjectValue="";

    QString mainSearchValue = searchTextField->text();
    QString str2 = QString("https://www.googleapis.com/books/v1/volumes?q="+mainSearchValue);

    if(authorBox->isChecked()){
        authorValue="inauthor:"+authorTextField->text();
        str2+="+"+authorValue;
    }

    if(titleBox->isChecked()){
        titleValue = "intitle:"+titleTextField->text();
        str2+="+"+titleValue;
    }

    if(subjectBox->isChecked())
    {
        subjectValue = "subject:"+subjectTextField->text();
        str2+="+"+subjectValue;
    }
    if(publisherBox->isChecked()){
        publisherValue = "inpublisher:"+publisherTextField->text();
        str2+="+"+publisherValue;
    }
    //QString str = QString("https://www.googleapis.com/books/v1/volumes?q=flowers+inauthor:keyes");
    //QString str2 = QString("https://www.googleapis.com/books/v1/volumes?q="+mainSearchValue+"+"+
                          //authorValue);//+"+"+titleValue+"+"+subjectValue+"+"+publisherValue);
    qDebug() << str2;
    networkAccessManager->get(QNetworkRequest(QUrl(str2)));


    qDebug() << "getting val";
    //networkAccessManager->get(QNetworkRequest(QUrl("https://httpbin.org/get")));
}

void MainWindow::replyFinished(QNetworkReply * x)
{

        titleVec.clear();
        authorsVec.clear();
        languageVec.clear();
        averageRatingVec.clear();
        publisherVec.clear();
        publishedDateVec.clear();

        QByteArray answer = x->readAll();
        QString ansUrl = x->url().toString();
        QJsonDocument doc(QJsonDocument::fromJson(answer));

        //1
        QJsonObject json = doc.object();


        //2
        QJsonValue itemsValue = json.value(QString("items"));
        insideItemsValue = itemsValue.toArray();


        //3
        QVector <QJsonObject > vectorInsideItemsValue;
        foreach(const auto& c, insideItemsValue)
        {
            vectorInsideItemsValue.append(c["volumeInfo"].toObject());

        }

        //4

        foreach(const auto& val, vectorInsideItemsValue)
        {
            QJsonArray authorX = val["authors"].toArray();
            QStringList authors;
            foreach(const auto& x, authorX)
            {
                authors << QVariant(x).toString();
            }
            authorsVec.append(authors);
            averageRatingVec.append(QVariant(val["averageRating"]).toDouble());
            titleVec.append(QVariant(val["title"]).toString());
            languageVec.append(QVariant(val["language"]).toString());
            publishedDateVec.append(QVariant(val["publishedDate"]).toString());
            publisherVec.append(QVariant(val["publisher"]).toString());

            /*
            qDebug() << "Rate:" << val["averageRating"]<< " " <<averageRating;
            qDebug() << "Title:" << val["title"] << " " <<title;
            qDebug() << "Language:" << language;
            qDebug() << "Published Date:" << val["publishedDate"] <<" " <<publishedDate;
            qDebug() << "Publisher:" << publisher;
            */
        }
        emit collectedAllData();

}

void MainWindow::createTableGrid()
{
    m_pTableWidget = new QTableWidget();
   // m_pTableWidget->setRowCount(insideItemsValue.size());
    m_pTableWidget->setColumnCount(6);
    m_TableHeader<<"Authors"<<"Title"<<"Rate"<<"Publish year"<<"Publisher"<<"Language";
    m_pTableWidget->setHorizontalHeaderLabels(m_TableHeader);
    m_pTableWidget->horizontalHeader();
    m_pTableWidget->resizeColumnsToContents();
    //m_pTableWidget->setFixedSize(m_pTableWidget->horizontalHeader()->length()+2*m_pTableWidget->verticalHeader()->width(),
    //                               m_pTableWidget->verticalHeader()->length()+m_pTableWidget->horizontalHeader()->height());
    //m_pTableWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    //m_pTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //m_pTableWidget->resizeColumnsToContents();
    //m_pTableWidget->resizeRowsToContents();
    //m_pTableWidget->horizontal
    //m_pTableWidget->setFixedSize(m_pTableWidget->horizontalHeader()->length()+2*m_pTableWidget->verticalHeader()->width(),
    //                           m_pTableWidget->verticalHeader()->length()+m_pTableWidget->horizontalHeader()->height());

    //m_pTableWidget->setFixedSize(1000,600);
 //updateTable();

 QVBoxLayout* tableLayout = new QVBoxLayout;
 tableLayout->addWidget(m_pTableWidget);
 this->grid->addLayout(tableLayout,1,0);
 this->adjustSize();
}

void MainWindow::updateTable()
{


        qDebug() << "Ilosc wierszy:" <<m_pTableWidget->rowCount();
        qDebug() << "Ilosc insideItemsValue:" << insideItemsValue.size();
        m_pTableWidget->setRowCount(insideItemsValue.size());
        for(int j=0;j<insideItemsValue.size();j++)
        {
            QString authors;
            for(int i=0;i<authorsVec[j].size();i++)
            {
                authors+=authorsVec[j].at(i);
                if(i<authorsVec[j].size()-1)
                {
                    authors+=", ";
                }
            }
            //authors =
            QTableWidgetItem* authorsItem = new QTableWidgetItem(authors);
            m_pTableWidget->setItem(j,0,authorsItem);

            QTableWidgetItem* titleItem = new QTableWidgetItem(titleVec[j]);
            m_pTableWidget->setItem(j,1, titleItem);

            QTableWidgetItem* rateItem = new QTableWidgetItem(QString::number(averageRatingVec[j]));
            m_pTableWidget->setItem(j,2, rateItem);

            QTableWidgetItem* publishedDateItem = new QTableWidgetItem(publishedDateVec[j]);
            m_pTableWidget->setItem(j,3, publishedDateItem);

            QTableWidgetItem* publisherItem = new QTableWidgetItem(publisherVec[j]);
            m_pTableWidget->setItem(j,4, publisherItem);

            QTableWidgetItem* languageItem = new QTableWidgetItem(languageVec[j]);
            m_pTableWidget->setItem(j,5, languageItem);
        }
        m_pTableWidget->viewport()->update();
        m_pTableWidget->setFixedSize(1000,350);
        m_pTableWidget->resizeColumnsToContents();
        m_pTableWidget->resizeRowsToContents();
        m_pTableWidget->setWordWrap(true);

        //m_pTableWidget->setFixedSize(m_pTableWidget->horizontalHeader()->length()+2*m_pTableWidget->verticalHeader()->width(),

        //                               m_pTableWidget->verticalHeader()->length()+2*m_pTableWidget->horizontalHeader()->height());

}
