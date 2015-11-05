#ifndef MODELITEM_H
#define MODELITEM_H

#include <QObject>

class ModelItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal InletFlowrate MEMBER m_InletFlowrate READ getInletFlowrate WRITE setInletFlowrate)
public:
    explicit ModelItem(QObject *parent = 0);
    ~ModelItem();
private:
    qreal m_InletFlowrate; // Inlet Volume Flowrate
signals:

public slots:
private slots:
    void setInletFlowrate(qreal Value);
    qreal getInletFlowrate();
};

#endif // MODELITEM_H
