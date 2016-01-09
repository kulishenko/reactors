#include "schemapipeline.h"

SchemaPipeline::SchemaPipeline() : SchemaItem(),
    m_From(nullptr), m_To(nullptr), LinePath(nullptr), p_Arrow(nullptr),
    m_FromElementId(0), m_ToElementId(0)
{

}

SchemaPipeline::SchemaPipeline(SchemaItem* From, SchemaItem* To) :
    SchemaItem(), m_From(From), m_To(To), LinePath(nullptr), p_Arrow(nullptr),
    m_FromElementId(From->property("ElementId").toInt()),
    m_ToElementId(To->property("ElementId").toInt())
{
    drawLine();

    // Pointer to the next item in schema
    From->setDescedant(To);
    connect(From, SIGNAL(moved()), this, SLOT(drawLine()));
    connect(To, SIGNAL(moved()), this, SLOT(drawLine()));
}

SchemaPipeline::~SchemaPipeline()
{

}

void SchemaPipeline::setFromElementId(int Value)
{
    m_FromElementId = Value;
    SchemaItem* From = parent()->getItemByElementId(Value);
    if(From) {
        connect(From, SIGNAL(moved()), this, SLOT(drawLine()));
        m_From = From;
    }

    if(m_From && m_To) drawLine();
}

void SchemaPipeline::setToElementId(int Value)
{
    m_ToElementId = Value;
    SchemaItem* To = parent()->getItemByElementId(Value);
    if(To) {
        connect(To, SIGNAL(moved()), this, SLOT(drawLine()));
        m_To = To;
    }

    if(m_From && m_To) drawLine();
}

void SchemaPipeline::drawLine()
{
    if(!m_From->getDescedant()) m_From->setDescedant(m_To); // Remove (make via SIGNAL-SLOT)

    QPainterPath* OldPath;
    OldPath = LinePath;
    LinePath = new QPainterPath();

    qreal ArrowSize = 5.0f;
    if(!p_Arrow) {

        p_Arrow = new QGraphicsPolygonItem(QPolygonF(QVector<QPointF>() << QPointF( 0, 0 )
                                                     << QPointF( 0, ArrowSize )
                                                     << QPointF( 1.75* ArrowSize , 0.5* ArrowSize  )
                                                     << QPointF(0,0)), this);
        p_Arrow->setPen( QPen(Qt::black) );
        p_Arrow->setBrush( Qt::black );
    }

    QPointF FromPos = m_From->getOutletPort()->scenePos();
    QPointF ToPos = m_To->getInletPort()->scenePos();

    LinePath->moveTo(FromPos);
    QPointF PrevPoint(FromPos);

    if((FromPos.x() != ToPos.x()) || (FromPos.y() != ToPos.y()))
    {
        if(fabs(m_From->getOutletPort()->getAngle() - m_To->getInletPort()->getAngle()) > 1e-2)
        {
            LinePath->lineTo(ToPos.x(), FromPos.y());
            PrevPoint = QPointF(ToPos.x(), FromPos.y());
        }
        else if(fmod(m_From->getOutletPort()->getAngle(), 180) < 1e-2)
        {
            LinePath->lineTo((ToPos.x() + FromPos.x())/2,  FromPos.y());
            LinePath->lineTo((ToPos.x() + FromPos.x())/2,  ToPos.y());
            PrevPoint = QPointF((ToPos.x() + FromPos.x())/2,  ToPos.y());
        }
        else // if()
        {
            LinePath->lineTo(FromPos.x(),(ToPos.y() + FromPos.y())/2);
            LinePath->lineTo(ToPos.x(), (ToPos.y() + FromPos.y())/2);
            PrevPoint = QPointF(ToPos.x(), (ToPos.y() + FromPos.y())/2);
        }
    }
    LinePath->lineTo(ToPos);
    setPath(*LinePath);

    p_Arrow->setPos(ToPos);

    QLineF LastLine(PrevPoint, ToPos);
    p_Arrow->setRotation(LastLine.angle() + 180);
    p_Arrow->moveBy(p_Arrow->boundingRect().center().x() , -p_Arrow->boundingRect().center().y());

    p_Arrow->setVisible(false);

    if(LinePath) delete OldPath;
}

void SchemaPipeline::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore();
}

void SchemaPipeline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore();
}

void SchemaPipeline::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(SchemaMode == RunMode::Edit) {
        if(isSelected()) // TODO: Fix
            setSelected(false);
        else {
            parent()->deSelectAllItems();
            setSelected(true);
        }
    }
    event->ignore();
}
