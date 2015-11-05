#include "modelitem.h"

ModelItem::ModelItem(QObject *parent) : QObject(parent)
{

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
