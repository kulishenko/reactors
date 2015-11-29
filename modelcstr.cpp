#include "modelcstr.h"

ModelCSTR::ModelCSTR(SchemaItem *parent) : ModelItem(parent), p_Tau(nullptr)
{
    m_Level = 0;
    m_Volume = 0;
    m_isWorking = false;
}

ModelCSTR::~ModelCSTR()
{

}

qreal ModelCSTR::getTau()
{
    return 0.084 / (getInletFlowrate()/ 10) * 1000 * 3600;
}


