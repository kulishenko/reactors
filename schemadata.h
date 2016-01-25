#ifndef SCHEMADATA_H
#define SCHEMADATA_H
#include <QVector>
#include <QDebug>
#include <QObject>
#include <pfdcontrol.h>

class TISModel;
class SchemaData
{
    friend class TISModel;
    friend class AxialdispersionModel;
public:
    SchemaData(PFDControl *Ctrl);
    ~SchemaData();
    void calcM0();
    qreal getM0();
    qreal getAvgTau();
    qreal getTau() const;
    qreal getNc() const;
    qreal getM2theta() const;
    qreal getSigma2theta() const;
    void calcAvgTau();
    void calcConc();
    void calcDimConc();
    void calcDimTime();
    void calcM2theta();
    void estimateNumCells();
    void SmoothData();
    qreal getConcAt(const size_t i) const;
    qreal getSConcAt(const size_t i) const;
    qreal getDimTimeAt(const size_t i) const;
    const QVector<qreal> &getSConc() const;
    const QVector<qreal> &getDimConc() const;
    const QVector<qreal> &getDimExpConc() const;
    const QVector<qreal> &getDimTime() const;
    const QVector<qreal> &getSimConc(const size_t i) const;
    size_t getSimConcCount() const;
    void calcDimExpConc();
    qreal getDimExpConcAt(const size_t i) const;
    const QString &getSimMethod(const int i) const;
    void addSimData(const QVector<qreal> & simData, const QString& name = QString());
private:
    void setSimMethod(QString name);
    const QVector<qreal> &p_ExpDataTime;
    const QVector<qreal> &p_ExpDataConductivity;
    int m_NumCascade;
    qreal m_Flowrate; // Volume flowrate, L/hr
    size_t m_DataRes; // Data resolution
    int i_t0;
    qreal t0; // Zero time, s
    qreal tend; // End time, s
    qreal m_tau; // Residence time, s
    qreal *avg_tau; // Average residence time, s
    qreal *M0; // Initial moment
    qreal M2t, M2theta; // 2-nd order moments
    qreal sigma2theta;
    qreal Nc; // Number of cells
    qreal Calibrate(qreal x) const;
    qreal dt(const size_t i) const;
    qreal dim_dt(const size_t i) const;
    QVector<qreal>::const_iterator t_0() const;
    QVector<qreal>::const_iterator t_last() const;
    QVector<qreal> Conc, SConc; // Tracer concentration, kmol/m3
    QVector<qreal> DimTime; // Dimensionless time (a.k.a. theta)
    QVector<qreal> DimConc; // Dimensionless tracer concentration (a.k.a. C_theta)
    QVector<qreal> DimExpConc; // Dimensionless experimental (unsmoothered) concentration
    QVector<QVector<qreal>> SimConc; // Simulation results - Tracer concentration
    QVector<QString> SimMethod;
};

#endif // SCHEMADATA_H
