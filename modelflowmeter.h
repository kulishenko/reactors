#ifndef MODELFLOWMETER_H
#define MODELFLOWMETER_H
#include "modelitem.h"

class ModelFlowmeter : public ModelItem
{
    Q_OBJECT
    Q_PROPERTY(qreal Value MEMBER m_Value READ getValue WRITE setValue)
public:
    ModelFlowmeter();
    ~ModelFlowmeter();
private:
    qreal m_Value; // Displaying value (relative to Max)
private slots:
    qreal getValue();
    void setValue(qreal Value);
};

#endif // MODELFLOWMETER_H
