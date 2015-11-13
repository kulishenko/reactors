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
void ModelItem::setInletFlowrate(const qreal Value)
{
    m_InletFlowrate = Value;
}

qreal ModelItem::getInletFlowrate() const
{
    return m_InletFlowrate;
}

void ModelItem::setMaxFlowrate(const qreal Value)
{
    m_MaxFlowrate = Value;
}

qreal ModelItem::getMaxFlowrate() const
{
    return m_MaxFlowrate;
}

int ModelItem::getElementId() const
{
    return m_ElementId;
}
