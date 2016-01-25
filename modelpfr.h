#ifndef MODELPFR_H
#define MODELPFR_H
#include <modelitem.h>

class ModelPFR : public ModelItem
{
    friend class SchemaCSTR;
    Q_OBJECT
    Q_PROPERTY(qreal Level MEMBER m_Level)
    Q_PROPERTY(bool isWorking MEMBER m_isWorking)
public:
    ModelPFR();
private:
    qreal m_Level; // Liquid Phase Fraction
    qreal m_Volume; // Vessel volume in L
    bool m_isWorking;
};

#endif // MODELPFR_H

