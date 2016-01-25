#ifndef SCHEMAPFR_H
#define SCHEMAPFR_H
#include <schemaitem.h>
#include <modelpfr.h>
#include <QPainterPath>

class SchemaPFR : public SchemaItem
{
    Q_OBJECT
public:
    SchemaPFR(qreal Width = 50.0f, qreal Height = 400.0f, QObject* parent = 0);
    ~SchemaPFR();
    enum class PFRIcon {Empty, FixedBed, Long};
    void setIcon(const PFRIcon icon);
private:
    PFRIcon m_IconType;
    QPainterPath* p_Path;
    ModelPFR* p_Model;
signals:
    void filled();
    void startedFeed();
public slots:
    void fill();
    void startFeed();
    void setFlowrate(const qreal Value);
};

#endif // SCHEMAPFR_H
