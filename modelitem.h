#ifndef MODELITEM_H
#define MODELITEM_H

#include <QObject>
/*!
 * \brief Базовый класс моделей, описывающих логику элементов схемы
 */
class ModelItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal InletFlowrate MEMBER m_InletFlowrate READ getInletFlowrate WRITE setInletFlowrate)
    Q_PROPERTY(qreal MaxFlowrate MEMBER m_MaxFlowrate READ getMaxFlowrate WRITE setMaxFlowrate)
    Q_PROPERTY(int ElementId MEMBER m_ElementId READ getElementId CONSTANT)
    friend class SchemaCSTR;
public:
    explicit ModelItem(QObject *parent = 0);
    ~ModelItem();
    static int s_ElementId;
private:
    qreal m_InletFlowrate; // Inlet Volume Flowrate, L/hr
    qreal m_MaxFlowrate; // MaxFlowrate, L/hr
    const int m_ElementId;
signals:

public slots:
protected slots:
    void setInletFlowrate(const qreal Value);
    qreal getInletFlowrate() const;
    void setMaxFlowrate(const qreal Value);
    qreal getMaxFlowrate() const;
    int getElementId() const;
};
#endif // MODELITEM_H
