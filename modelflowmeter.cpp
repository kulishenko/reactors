#include "modelflowmeter.h"

ModelFlowmeter::ModelFlowmeter() : ModelItem()
{
    m_Value = 0;
}

ModelFlowmeter::~ModelFlowmeter()
{

}

qreal ModelFlowmeter::getValue()
{
    return m_Value;
}

void ModelFlowmeter::setValue(qreal Value)
{
    m_Value = Value;
}

