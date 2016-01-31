#ifndef MODELCSTR_H
#define MODELCSTR_H
#include <modelitem.h>
#include <schemaitem.h>
#include <QFile>
#include <QDebug>
/*!
 * \brief Класс модели, описывающей поведение реактора с мешалкой
 */
class ModelCSTR : public ModelItem
{
    friend class SchemaCSTR;
    Q_OBJECT
    Q_PROPERTY(qreal Level MEMBER m_Level)
    Q_PROPERTY(bool isWorking MEMBER m_isWorking)
public:
    ModelCSTR(SchemaItem* parent = 0);
    ~ModelCSTR();
private:
    qreal m_Level; // Liquid Phase Fraction
    qreal m_Volume; // Vessel volume in L
    bool m_isWorking;
    QVector<qreal> *p_Tau;
    qreal getTau();
};

#endif // MODELCSTR_H
