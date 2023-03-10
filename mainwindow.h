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
#include <QDesktopWidget>
#include <QPrintDialog>
#include <QPrinter>
#include <QToolBar>
#include "schemacstr.h"
#include "schemapipeline.h"
#include "schemaflowmeter.h"
#include "schemastream.h"
#include "schemaview.h"
#include "schemadata.h"
#include "schemacell.h"
#include "tismodel.h"
#include "axialdispersionmodel.h"
#include "schemadb.h"
#include "schemalogger.h"
#include "modelcstr.h"
#include "modelflowmeter.h"
#include "schemaconfig.h"
#include "schemascene.h"
#include <schemapfr.h>
#include <qwt-mml/qwt_mml_document.h>
#include <formulaitem.h>
#include <qgraphicsplot.h>
#include <QGraphicsGridLayout>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QGraphicsPixmapItem>
#include <formulalayoutitem.h>
#include <qgraphicsplotlayoutitem.h>
#include <qgraphicspixmaplayoutitem.h>

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

/*!
 * \brief Класс главного окна приложения
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
//    Q_PROPERTY(RunMode runMode MEMBER m_RunMode)
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MainWindow &operator<<(const QString& Event);
//private:
    //    bool eventFilter(QObject *, QEvent *);
protected:
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
private slots:
    void exportFinished(bool result);
    void onlineMode();
    void offlineMode();
    void editMode();
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
    SchemaScene *m_scene;
    QString m_sSettingsFile;

    SchemaLogger EventLog;

    SchemaView* graphicsView;

    SchemaData *p_Data;

    QVector<SchemaCSTR*> reactorItems;

    void loadSettings();
    void saveSettings();

    void createActions();
    void createMenus();
    void createToolBars();
    void createDockWindows();
    void createSchemaView();
    void createSchemaScene();
    void initControl();

    void loadSceneFromFile(const QString &filename = ":/resources/SchemaCascadeCSTR.xml");

    void loadSimDataFromFile(const QString &filename);

    void connectItems(int ElementIdFrom, const int ElementIdTo);

    QColor bgColor;
    QLinearGradient* p_bgGradient;

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

    /*
    QAction *boldAct;
    QAction *italicAct;
    QAction *leftAlignAct;
    QAction *rightAlignAct;
    QAction *justifyAct;
    QAction *centerAct;
    QAction *setLineSpacingAct;
    QAction *setParagraphSpacingAct; */

    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *fitInViewAct;

    QAction *importFromServerAct;
    QAction *exportToServerAct;

    QAction *paramEstimAct;

    QAction *playbackAct;
    QAction *onlineAct;
    QAction *editModeAct;

    QAction *aboutAct;
    QAction *aboutQtAct;
    QLabel *infoLabel;


    QAction *addCSTRAct;
    QAction *addPFRAct;
    QAction *addFlowmeterAct;
    QAction *addValveAct;
    QAction *addStreamAct;
    QAction *addCellAct;

    QAction *attachModeAct;

    QCustomPlot *plotWidget;
    QTableWidget *tableWidget;
    QVideoWidget *videoWidget;
    QListWidget *eventsWidget;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *controlToolBar;
    QMediaPlayer *MediaPlayer;
    QThread* thread;
    QTimer* timer;
    PFDControl* Control;
    SchemaDB* database;

    bool isSimRun;
    bool isDBConnected;
    bool m_isStarted;

};

#endif // MAINWINDOW_H
