#include "modelcstr.h"

ModelCSTR::ModelCSTR() : ModelItem()
{
    m_Level = 0;
    m_Volume = 0;
    m_isWorking = false;
}

ModelCSTR::~ModelCSTR()
{

}

void ModelCSTR::setLevel(qreal Value)
{
    m_Level = Value;
}

qreal ModelCSTR::getLevel()
{
    return m_Level;
}

void ModelCSTR::setWorking(bool Value)
{
    m_isWorking = Value;
}

bool ModelCSTR::isWorking()
{
    return m_isWorking;
}

