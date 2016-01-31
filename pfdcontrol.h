#ifndef PFDCONTROL_H
#define PFDCONTROL_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QGraphicsPathItem>
#include <schemaitem.h>
#include <QVector>
#include <QList>
#include <QFile>
#include <QApplication>

class SchemaItem;

/*!
 * \brief Класс, описывающий логику работы мнемосхемы (Control)
 *
 * Осуществляет:
 * - управление логикой мнемосхемы;
 * - воспроизведение результатов предыдущих экспериментов;
 * - двусторонний обмен данными с серверным модулем.
 */

class PFDControl : public QObject
{
    Q_OBJECT

public:
    explicit PFDControl(QObject *parent = 0);
    ~PFDControl();
//    QVector<SchemaCSTR*> *reactorItems;
//    void addItem(SchemaItem *item);
    void calcTau();
    void setNumCascade(int Value);
    qreal getCurrentTime();
    void addPoint(qreal Time, qreal Value);
    const QVector<qreal> &getTime() const;
    const QVector<qreal> &getParameter() const;
    qreal getTimeAt(const size_t i) const;
    qreal getParameterAt(const size_t i) const;
    qreal getTauAt(const size_t i) const; // TODO: Replace with ModelCSTR
    int getCount() const;
    qreal getFlowrate() const;
    qreal getPlaybackFlowrate() const;
    void setPlaybackFileName(const QString &str);
    const QString &getPlaybackFileName() const;
    int getNumCascade() const;
private:
    qreal m_Flowrate; //!< Текущий объемный расход вещества, л/ч
    qreal m_PlaybackFlowrate; //!< Объемный расход вещества для записи опыта, л/ч
    QString m_PlaybackFileName; //!< Имя файла записи опыта (при чтении исходных данных из файла)
    int m_NumCascade; //!< Число реакторов в каскаде (по заданию)
    QVector<qreal> m_Time; //!< Массив значений времени, соответствующего экспериментальным данным
    QVector<qreal> m_Parameter; //!< Массив значений экспериментального параметра
    bool isStarted; //!< Установка запущена
    bool isFlowrateSet; //!< Установлен требуемый объемный расход (режим воспроизведения)
    qreal TimeNow; //!< Текущее время с момента начала эксперимента
    QVector<qreal> Tau; // Transient times for CSTRs (temp) -> to be moved to ModelCSTR
signals:
    void setLevel();
    void doSim(); //!< Выполняет шаг воспроизведения опыта
    void startSim();
    void started();
public slots:
    void tick(); //!< Опрос параметров
    void flowrate_increased(); //!< Увеличение объемного расхода
    void flowrate_decreased(); //!< Уменьшение объемного расхода
    void Start(); //!< Запуск воспроизведения опыта
    void setFlowrate(qreal Value); //!< Установить объемный расход, л/ч
    void setPlaybackFlowrate(qreal Value); //!< Установить объемный расход для режима воспроизведения, л/ч
};

#endif // PFDCONTROL_H
