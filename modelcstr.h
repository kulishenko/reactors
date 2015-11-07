#ifndef MODELCSTR_H
#define MODELCSTR_H
#include <modelitem.h>

class ModelCSTR : public ModelItem
{
    Q_OBJECT
    Q_PROPERTY(qreal Level MEMBER m_Level)
    Q_PROPERTY(bool isWorking MEMBER m_isWorking)
public:
    ModelCSTR();
    ~ModelCSTR();
private:
    qreal m_Level; // Liquid Phase Fraction
    qreal m_Volume; // Vessel volume in L
    bool m_isWorking;
};

#endif // MODELCSTR_H
