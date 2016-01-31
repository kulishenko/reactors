#ifndef SCHEMAPFR_H
#define SCHEMAPFR_H
#include <schemaitem.h>
#include <modelpfr.h>
#include <QPainterPath>

/*!
 * \brief Класс, описывающий графический элемент, обозначающий трубчатый реактор
 */
class SchemaPFR : public SchemaItem
{
    Q_OBJECT
public:
    SchemaPFR(qreal Width = 50.0f, qreal Height = 400.0f, QObject* parent = 0);
    ~SchemaPFR();
    enum class PFRIcon {
        Empty, //!< Полый трубчатый реактор, L/d < 50
        PackedBed, //!< Трубчатый реактор насадками, L/d < 50
        Long //!< Полый трубчатый реактор, L/d > 50
    }; //!< Тип элемента
    void setIcon(const PFRIcon icon);
private:
    PFRIcon m_IconType; //!< Пиктограмма элемента
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
