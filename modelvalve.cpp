#include "modelvalve.h"

ModelValve::ModelValve() : ModelItem()
{
    m_Position = 0.f;
    m_MaxFlowrate = 0.f;
}

ModelValve::~ModelValve()
{

}

qreal ModelValve::getPosition()
{
    return m_Position;
}

void ModelValve::setPosition(qreal Value)
{
    m_Position = Value;
}

qreal ModelValve::getMaxFlowrate()
{
    return m_MaxFlowrate;
}

void ModelValve::setMaxFlowrate(qreal Value)
{
    m_MaxFlowrate = Value;
}

