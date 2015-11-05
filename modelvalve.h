#ifndef MODELVALVE_H
#define MODELVALVE_H
#include "modelitem.h"

class ModelValve : public ModelItem
{
    Q_OBJECT
    Q_PROPERTY(qreal Position MEMBER m_Position READ getPosition WRITE setPosition)
    Q_PROPERTY(qreal MaxFlowrate MEMBER m_MaxFlowrate READ getMaxFlowrate WRITE setMaxFlowrate)
public:
    ModelValve();
    ~ModelValve();
private:
    qreal m_Position; // Relative Volume Flowrate
    qreal m_MaxFlowrate; // Maximum Flowrate, L/min
private slots:
    qreal getPosition();
    void setPosition(qreal Value);
    qreal getMaxFlowrate();
    void setMaxFlowrate(qreal Value);
};

#endif // MODELVALVE_H
