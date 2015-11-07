#include "modelitem.h"
int ModelItem::s_ElementId = 0;
ModelItem::ModelItem(QObject *parent) : QObject(parent) , m_ElementId(s_ElementId++)
{
    m_InletFlowrate = 0.f;
    m_MaxFlowrate = 0.f;
}

ModelItem::~ModelItem()
{

}
void ModelItem::setInletFlowrate(qreal Value)
{
    m_InletFlowrate = Value;
}

qreal ModelItem::getInletFlowrate()
{
    return m_InletFlowrate;
}

void ModelItem::setMaxFlowrate(qreal Value)
{
    m_MaxFlowrate = Value;
}

qreal ModelItem::getMaxFlowrate()
{
    return m_MaxFlowrate;
}

int ModelItem::getElementId()
{
    return m_ElementId;
}
