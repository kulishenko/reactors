#ifndef MODELITEM_H
#define MODELITEM_H

#include <QObject>

class ModelItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal InletFlowrate MEMBER m_InletFlowrate READ getInletFlowrate WRITE setInletFlowrate)
    Q_PROPERTY(qreal MaxFlowrate MEMBER m_MaxFlowrate READ getMaxFlowrate WRITE setMaxFlowrate)
    Q_PROPERTY(int ElementId MEMBER m_ElementId READ getElementId CONSTANT)
public:
    explicit ModelItem(QObject *parent = 0);
    ~ModelItem();
    static int s_ElementId;
private:
    qreal m_InletFlowrate; // Inlet Volume Flowrate, L/hr
    qreal m_MaxFlowrate; // MaxFlowrate, L/hr
    const int m_ElementId;
signals:

public slots:
private slots:
    void setInletFlowrate(qreal Value);
    qreal getInletFlowrate();
    void setMaxFlowrate(qreal Value);
    qreal getMaxFlowrate();
    int getElementId();
};
#endif // MODELITEM_H
