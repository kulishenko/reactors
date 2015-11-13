#include "modelvalve.h"

ModelValve::ModelValve() : ModelItem()
{
    m_Position = 0.f;
    m_MaxFlowrate = 0.f;
}

ModelValve::~ModelValve()
{

}

qreal ModelValve::getPosition() const
{
    return m_Position;
}

void ModelValve::setPosition(const qreal Value)
{
    m_Position = Value;
}

qreal ModelValve::getMaxFlowrate() const
{
    return m_MaxFlowrate;
}

void ModelValve::setMaxFlowrate(const qreal Value)
{
    m_MaxFlowrate = Value;
}

