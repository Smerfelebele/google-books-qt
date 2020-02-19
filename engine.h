#ifndef ENGINE_H
#define ENGINE_H

#include <QtNetwork>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QRect>
#include <QNetworkAccessManager>

class Engine : public QWidget
{
    Q_OBJECT

public:
    Engine();
    //~Engine();
    /*QGridLayout* grid;

    QNetworkAccessManager *networkAccessManager;
private slots:
    void handleSearchButton();
    void replyFinished(QNetworkReply* );
*/
};

#endif // ENGINE_H
