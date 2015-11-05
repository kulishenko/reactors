#ifndef MODELCSTR_H
#define MODELCSTR_H
#include <modelitem.h>

class ModelCSTR : public ModelItem
{
    Q_OBJECT
    Q_PROPERTY(qreal Level MEMBER m_Level READ getLevel WRITE setLevel)
    Q_PROPERTY(bool isWorking MEMBER m_isWorking READ isWorking WRITE setWorking)
public:
    ModelCSTR();
    ~ModelCSTR();
private:
    qreal m_Level; // Liquid Phase Fraction
    qreal m_Volume; // Vessel volume in L
    bool m_isWorking;
private slots:
    void setLevel(qreal Value);
    qreal getLevel();
    void setWorking(bool Value);
    bool isWorking();
};

#endif // MODELCSTR_H
