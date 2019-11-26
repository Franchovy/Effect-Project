#ifndef PORT_H
#define PORT_H

#include <QObject>

QT_BEGIN_NAMESPACE
class Effect;
class QComboBox;
class QPushButton;
QT_END_NAMESPACE

class Port : public QObject
{
    Q_OBJECT
public:
    explicit Port(QString name, Effect *parent = nullptr);

    bool isConnectedPortSet();
    Port* getConnectedPort();
    void setConnectedPort(Port* port);

    void setupConnectionSelect();
    QComboBox* getConnectionSelect(){return connectionSelect;}
    QPushButton* getDisconnectButton(){return disconnectButton;}
    virtual char* getData();

    QString getName(){return portName;}

    virtual int getConnectPortType(){return 0;}
    virtual int getPortType() {return 0;}

protected:
    Port* connectedPort = nullptr;
    QString portName;

    Effect* parentEffect;

    //temporary UI
    QComboBox* connectionSelect;
    QPushButton* disconnectButton;
signals:
    void sendConnectionSelect(int portType);
public slots:
    void disconnectPort();
};

#endif // PORT_H
