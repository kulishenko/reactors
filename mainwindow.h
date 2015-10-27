#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QVector>
#include <QPointF>
#include <schemacstr.h>
#include <schemaitem.h>
#include <schemavalve.h>
#include <QFileDialog>
#include <QListWidget>
#include "qcustomplot/qcustomplot.h"
#include <stdio.h>
#include <assert.h>
#include <QMovie>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTableWidget>
#include <QThread>
#include <QTimer>
#include <pfdcontrol.h>
#include <QObject>
#include <QMessageBox>
#include <QWindow>
#include <QLabel>
#include <QTextStream>
#include "schemacstr.h"
#include "schemapipeline.h"
#include "schemaflowmeter.h"
#include "schemastream.h"
#include "schemaview.h"
#include "schemadata.h"
#include "schemacell.h"
#include "modelcell.h"
#include "schemadb.h"
#include "schemalogger.h"

extern "C" {
#include <libxls/xls.h>
}

namespace Ui {
class MainWindow;
}

class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QListWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MainWindow &operator<<(const QString& Event);
//private:
    //    bool eventFilter(QObject *, QEvent *);
public slots:
    void exportFinished(bool result);
protected:
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
private slots:
    void newFile();
    void open();
    void save();
    void print();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void bold();
    void italic();
    void leftAlign();
    void rightAlign();
    void justify();
    void center();
    void setLineSpacing();
    void setParagraphSpacing();
    void about();
    void aboutQt();
    void updateWidgets();
    void Run();
    void zoomIn();
    void zoomOut();
    void fitInView();
    void paramEstimation();
    void importFromServerDlg();
    void importFromServer(bool result);
    void exportToServer();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_scene;
    QString m_sSettingsFile;

    SchemaLogger EventLog;

    SchemaView* graphicsView;

    SchemaData *Data;

    SchemaFlowmeter* flowmeterItem;
    SchemaValve* valveItem1;

    QVector<SchemaCSTR*> reactorItems;
    QVector<SchemaPipeline*> pipelineItems;
    void loadSettings();
    void saveSettings();

    void createActions();
    void createMenus();
    void createToolBars();
    void createDockWindows();
    void createSchemaView();
    void initControl();

    QColor bgColor;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *zoomMenu;
    QMenu *modeMenu;
    QMenu *dataMenu;
    QMenu *toolsMenu;
    QMenu *analysisMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;
    QActionGroup *alignmentGroup;
    QActionGroup *modeGroup;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *printAct;
    QAction *exitAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *boldAct;
    QAction *italicAct;
    QAction *leftAlignAct;
    QAction *rightAlignAct;
    QAction *justifyAct;
    QAction *centerAct;
    QAction *setLineSpacingAct;
    QAction *setParagraphSpacingAct;

    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *fitInViewAct;

    QAction *importFromServerAct;
    QAction *exportToServerAct;

    QAction *paramEstimAct;

    QAction *playbackAct;
    QAction *onlineAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QLabel *infoLabel;

    QCustomPlot *plotWidget;
    QTableWidget *tableWidget;
    QVideoWidget *videoWidget;
    QListWidget *eventsWidget;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QMediaPlayer *MediaPlayer;
    QThread* thread;
    QTimer* timer;
    PFDControl* Control;
    SchemaDB* database;

    bool isSimRun;
    bool isDBConnected;

};

#endif // MAINWINDOW_H
