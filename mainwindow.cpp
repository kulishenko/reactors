
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QApplication>
#include <QGridLayout>
#include <QEvent>
#include <QResizeEvent>
#include <QMessageBox>
#include <QDockWidget>
#include <QVideoSurfaceFormat>
#include <QGraphicsVideoItem>


extern "C" {
#include <stdio.h>
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_scene(nullptr), thread(nullptr), Control(nullptr), m_isStarted(false)
{

    //bgColor = QColor::fromRgb(240, 240, 240);
    bgColor = Qt::gray;
    //qRegisterMetaType<RunMode>("RunMode");

    ui->setupUi(this);

    createActions();
    createMenus();
    createToolBars();
    createDockWindows();

    //createSchemaScene();
    createSchemaView();
    loadSceneFromFile();

    QString message = tr("A context menu is available by right-clicking");
    statusBar()->showMessage(message);

    setWindowTitle(tr("Reactors Lab"));
    setMinimumSize(50, 50);

    this->setCentralWidget(graphicsView);
    this->adjustSize();

    loadSettings();
    m_isStarted = true;
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(cutAct);
    menu.addAction(copyAct);
    menu.addAction(pasteAct);
    menu.exec(event->globalPos());
}

void MainWindow::createActions()
{
    newAct = new QAction(QIcon(":/images/new.png"),tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"),tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    printAct = new QAction(QIcon(":/images/print.png"),tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the document"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    undoAct = new QAction(QIcon(":/images/undo.png"),tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last operation"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last operation"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    cutAct = new QAction(tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    boldAct = new QAction(tr("&Bold"), this);
    boldAct->setCheckable(true);
    boldAct->setShortcut(QKeySequence::Bold);
    boldAct->setStatusTip(tr("Make the text bold"));
    connect(boldAct, SIGNAL(triggered()), this, SLOT(bold()));

    QFont boldFont = boldAct->font();
    boldFont.setBold(true);
    boldAct->setFont(boldFont);

    italicAct = new QAction(tr("&Italic"), this);
    italicAct->setCheckable(true);
    italicAct->setShortcut(QKeySequence::Italic);
    italicAct->setStatusTip(tr("Make the text italic"));
    connect(italicAct, SIGNAL(triggered()), this, SLOT(italic()));

    QFont italicFont = italicAct->font();
    italicFont.setItalic(true);
    italicAct->setFont(italicFont);

    setLineSpacingAct = new QAction(tr("Set &Line Spacing..."), this);
    setLineSpacingAct->setStatusTip(tr("Change the gap between the lines of a "
                                       "paragraph"));
    connect(setLineSpacingAct, SIGNAL(triggered()), this, SLOT(setLineSpacing()));

    setParagraphSpacingAct = new QAction(tr("Set &Paragraph Spacing..."), this);
    setParagraphSpacingAct->setStatusTip(tr("Change the gap between paragraphs"));
    connect(setParagraphSpacingAct, SIGNAL(triggered()),
            this, SLOT(setParagraphSpacing()));


    zoomInAct = new QAction(tr("Zoom &In"),this);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setStatusTip(tr("Zoom In the schema"));
    connect(zoomInAct,SIGNAL(triggered()),this,SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out"),this);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setStatusTip(tr("Zoom Out the schema"));
    connect(zoomOutAct,SIGNAL(triggered()),this,SLOT(zoomOut()));

    fitInViewAct = new QAction(tr("&Fit In View"),this);
  //  fitInViewAct->setShortcut(QKeySequence::);
    fitInViewAct->setStatusTip(tr("Fit the schema in view"));
    connect(fitInViewAct,SIGNAL(triggered()),this,SLOT(fitInView()));

    exportToServerAct = new QAction(tr("&Export to server..."),this);
    exportToServerAct->setStatusTip(tr("Send the experimental data to server"));
    exportToServerAct->setDisabled(true);
    connect(exportToServerAct,SIGNAL(triggered()),this,SLOT(exportToServer()));

    importFromServerAct = new QAction(tr("&Import from server..."),this);
    importFromServerAct->setStatusTip(tr("Get the experimental data from server"));
    connect(importFromServerAct,SIGNAL(triggered()),this,SLOT(importFromServerDlg()));

    paramEstimAct = new QAction(tr("&Parameter estimation..."),this);
    paramEstimAct->setStatusTip(tr("Estimate parameters from experimental data"));
    paramEstimAct->setDisabled(true);
    connect(paramEstimAct,SIGNAL(triggered()),this,SLOT(paramEstimation()));

    playbackAct = new QAction(tr("&Offline"),this);
    playbackAct->setCheckable(true);
    playbackAct->setStatusTip(tr("Enable the playback mode"));
    connect(playbackAct, SIGNAL(triggered()), this, SLOT(offlineMode()));

    onlineAct = new QAction(tr("&Online"),this);
    onlineAct->setCheckable(true);
    onlineAct->setStatusTip(tr("Enable the online mode and connect to remote Lab server"));
    connect(onlineAct, SIGNAL(triggered()), this, SLOT(onlineMode()));

    editModeAct = new QAction(tr("&Editor"),this);
    editModeAct->setCheckable(true);
    editModeAct->setStatusTip(tr("Enable the edit mode"));
    connect(editModeAct, SIGNAL(triggered()), this, SLOT(editMode()));

    modeGroup = new QActionGroup(this);
    modeGroup->addAction(playbackAct);
    modeGroup->addAction(onlineAct);
    modeGroup->addAction(editModeAct);
    playbackAct->setChecked(true);
    onlineAct->setDisabled(true);
    //m_RunMode = RunMode::Offline;

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(aboutQt()));

    leftAlignAct = new QAction(tr("&Left Align"), this);
    leftAlignAct->setCheckable(true);
    leftAlignAct->setShortcut(tr("Ctrl+L"));
    leftAlignAct->setStatusTip(tr("Left align the selected text"));
    connect(leftAlignAct, SIGNAL(triggered()), this, SLOT(leftAlign()));

    rightAlignAct = new QAction(tr("&Right Align"), this);
    rightAlignAct->setCheckable(true);
    rightAlignAct->setShortcut(tr("Ctrl+R"));
    rightAlignAct->setStatusTip(tr("Right align the selected text"));
    connect(rightAlignAct, SIGNAL(triggered()), this, SLOT(rightAlign()));

    justifyAct = new QAction(tr("&Justify"), this);
    justifyAct->setCheckable(true);
    justifyAct->setShortcut(tr("Ctrl+J"));
    justifyAct->setStatusTip(tr("Justify the selected text"));
    connect(justifyAct, SIGNAL(triggered()), this, SLOT(justify()));

    centerAct = new QAction(tr("&Center"), this);
    centerAct->setCheckable(true);
    centerAct->setShortcut(tr("Ctrl+E"));
    centerAct->setStatusTip(tr("Center the selected text"));
    connect(centerAct, SIGNAL(triggered()), this, SLOT(center()));

    alignmentGroup = new QActionGroup(this);
    alignmentGroup->addAction(leftAlignAct);
    alignmentGroup->addAction(rightAlignAct);
    alignmentGroup->addAction(justifyAct);
    alignmentGroup->addAction(centerAct);
    leftAlignAct->setChecked(true);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();

    viewMenu = menuBar()->addMenu(tr("&View"));
    zoomMenu = viewMenu->addMenu(tr("&Zoom"));
    zoomMenu->addAction(zoomInAct);
    zoomMenu->addAction(zoomOutAct);
    zoomMenu->addAction(fitInViewAct);

    modeMenu = menuBar()->addMenu(tr("&Mode"));
    modeMenu->addAction(playbackAct);
    modeMenu->addAction(onlineAct);
    modeMenu->addAction(editModeAct);

    dataMenu = menuBar()->addMenu(tr("&Data"));
    dataMenu->addAction(importFromServerAct);
    dataMenu->addAction(exportToServerAct);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(paramEstimAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

/*  formatMenu = editMenu->addMenu(tr("&Format"));
    formatMenu->addAction(boldAct);
    formatMenu->addAction(italicAct);
    formatMenu->addSeparator()->setText(tr("Alignment"));
    formatMenu->addAction(leftAlignAct);
    formatMenu->addAction(rightAlignAct);
    formatMenu->addAction(justifyAct);
    formatMenu->addAction(centerAct);
    formatMenu->addSeparator();
    formatMenu->addAction(setLineSpacingAct);
    formatMenu->addAction(setParagraphSpacingAct); */
}

void MainWindow::createDockWindows()
{


    QDockWidget *dock = new QDockWidget(tr("Video"), this);
    dock->setProperty("objectName", QString("Video"));

    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    videoWidget = new QVideoWidget(dock);
    MediaPlayer = new QMediaPlayer(this);
    QMediaPlaylist* playlist = new QMediaPlaylist(this);

    playlist->addMedia(QUrl::fromLocalFile(qApp->applicationDirPath() + "/00332.wmv"));
    playlist->setCurrentIndex(1);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    MediaPlayer->setPlaylist(playlist);
    MediaPlayer->setVideoOutput(videoWidget);
    MediaPlayer->setMuted(true);

    dock->setWidget(videoWidget);
    dock->setMinimumHeight(150);

    addDockWidget(Qt::RightDockWidgetArea, dock);

    dock = new QDockWidget(tr("Plot"), this);
    dock->setProperty("objectName", QString("Plot"));

    plotWidget = new QCustomPlot(dock);


    plotWidget->addGraph();
    plotWidget->xAxis->setLabel(tr("Time, s"));
    plotWidget->yAxis->setLabel(tr("Conductivity, mS/cm"));
    plotWidget->rescaleAxes();
    dock->setWidget(plotWidget);
    dock->setMinimumWidth(500);
    dock->setMaximumHeight(400);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    dock = new QDockWidget(tr("Table"), this);
    dock->setProperty("objectName", QString("Table"));

    tableWidget = new QTableWidget(1,2,dock);

    QPalette* palette = new QPalette();
    palette->setColor(QPalette::Highlight,Qt::green);
    tableWidget->setPalette(*palette);

    tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Time, s") << tr("Conductivity, mS/cm"));
    tableWidget->horizontalHeader()->setStretchLastSection(true);

    dock->setWidget(tableWidget);
    addDockWidget(Qt::RightDockWidgetArea,dock);

    dock = new QDockWidget(tr("Event Log"), this);
    dock->setProperty("objectName", QString("EventLog"));
    eventsWidget = new QListWidget(dock);
    dock->setWidget(eventsWidget);
    dock->setMaximumHeight(100);
    addDockWidget(Qt::BottomDockWidgetArea, dock);

    EventLog.setWidget(eventsWidget);
    EventLog  << tr("Program started");
}

void MainWindow::createSchemaView()
{

    graphicsView = new SchemaView(this);

    graphicsView->setScene(m_scene);
    graphicsView->viewport()->installEventFilter(this);
    graphicsView->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::createSchemaScene()
{
    /* FOR TESTING PURPOSES ONLY
    SchemaConfig Config;
    QFile file(qApp->applicationDirPath() + "/SchemaConfig.xml");
    file.remove();

    QList<SchemaItem*> schemaItems;

    m_scene = new SchemaScene(this);

    //m_scene->setBackgroundBrush(QBrush(bgColor, Qt::SolidPattern));

    SchemaStream* streamItem1 = new SchemaStream(50, 0, 400, 0);
    schemaItems.append(streamItem1);

    SchemaStream* streamItem2 = new SchemaStream(50, 1300, 650, 90);
    schemaItems.append(streamItem2);

    // Temp
    SchemaCell* MeasurementCell = new SchemaCell(100, 40, 1255, 550);
    schemaItems.append(MeasurementCell);

    SchemaValve* valveItem1 = new SchemaValve(30, 45, 122.5, 350, -90);
    schemaItems.append(valveItem1);


    for(int i = 1; i <= 5; i++){
        reactorItems.push_back(new SchemaCSTR(90, 120, i*200, i*70, 0.1, i-1));

        schemaItems.append(reactorItems.at(i-1));
       // m_scene->appendReactorItemElementId(reactorItems.at(i-1)->property("ElementId").toInt());
    }



    SchemaFlowmeter* flowmeterItem = new SchemaFlowmeter(25, 200, 125, 50, 0);

    schemaItems.append(flowmeterItem);


    QVector<SchemaPipeline*> pipelineItems;

    // Connecting Items
    pipelineItems.push_back(new SchemaPipeline(streamItem1, valveItem1));
    pipelineItems.push_back(new SchemaPipeline(valveItem1, flowmeterItem));

    valveItem1->Descedant = flowmeterItem;

    // Connecting CSTRs

    pipelineItems.push_back(new SchemaPipeline(flowmeterItem, reactorItems.at(0)));
    for(int i = 0; i < reactorItems.size() - 1; i++)
        pipelineItems.push_back(new SchemaPipeline(reactorItems.at(i), reactorItems.at(i+1)));

    pipelineItems.push_back(new SchemaPipeline(reactorItems.last(), MeasurementCell));
    pipelineItems.push_back(new SchemaPipeline(MeasurementCell, streamItem2));

    std::for_each(pipelineItems.cbegin(), pipelineItems.cend(),[&](SchemaPipeline* item){ schemaItems.append(item); });

    foreach(SchemaItem* item, schemaItems) {
        m_scene->addItem(item);
        Config.serializeObject(item, &file);
    }
    */
}

void MainWindow::initControl()
{

    QList<int> *reactorItemsElementId = m_scene->getReactorItemsList();


    for(int i = 0; i < reactorItemsElementId->size() - 1; i++)
    {
        connect(m_scene->getItemByElementId(reactorItemsElementId->at(i)), SIGNAL(startedFeed()),
                m_scene->getItemByElementId(reactorItemsElementId->at(i+1)), SLOT(fill()));
    }

    connect(m_scene->getItemByElementId(reactorItemsElementId->last()), SIGNAL(filled()),
            this, SLOT(Run()));

    //Control->addItem(flowmeterItem);
    SchemaItem* flowmeterItem = m_scene->getItemByElementId(m_scene->getFlowmeterItemElementId());

    connect(flowmeterItem, SIGNAL(establishedFlowrate(qreal)),
            Control, SLOT(setFlowrate(qreal)));
    //Control->calcTau();


    foreach(SchemaItem *item, m_scene->schemaItems()) {
        if(item != flowmeterItem)
            connect(flowmeterItem, SIGNAL(establishedFlowrate(qreal)),
                    item, SLOT(setFlowrate(qreal)));
        item->activate();
    }

    if(thread) thread->deleteLater();

    thread = new QThread(this);
    timer = new QTimer();
    timer->setInterval(1000);
    timer->moveToThread(thread);
    Control->moveToThread(thread);

    connect(thread, SIGNAL(started()), timer, SLOT(start()));
    connect(timer, SIGNAL(timeout()), Control, SLOT(tick()));

    connect(this, SIGNAL(destroyed()), thread, SLOT(quit()));

    SchemaItem* valveItem = m_scene->getItemByElementId(m_scene->getValveItemElementId());

   // connect(valveItem, SIGNAL(FlowIncrease()), Control, SLOT(flowrate_increased()));
   // connect(valveItem, SIGNAL(FlowDecrease()), Control, SLOT(flowrate_decreased()));

    connect(valveItem, SIGNAL(FlowrateChanged(qreal)), flowmeterItem, SLOT(setFlowrate(qreal)));

    connect(Control, SIGNAL(setLevel()), MediaPlayer, SLOT(play()));

    connect(Control, SIGNAL(setLevel()), m_scene->getItemByElementId(reactorItemsElementId->first()), SLOT(fill()));

    connect(Control, SIGNAL(doSim()), this, SLOT(updateWidgets()));
  //  connect(Control, SIGNAL(startSim()), this, SLOT(Run())); // Connect the last CSTR

    EventLog << tr("Schema controls are initialized");

    // ToDO: REMOVE THIS
    const QVector<qreal> &Time = Control->getTime();
    const QVector<qreal> &Conductivity = Control->getParameter();

    plotWidget->graph(0)->setData(Time, Conductivity);
    plotWidget->rescaleAxes();
    plotWidget->replot();
    plotWidget->graph(0)->clearData();
    plotWidget->replot();

    // ToDo: Should be activated at the end of experiment
    paramEstimAct->setDisabled(false);
    exportToServerAct->setDisabled(false);
    EventLog << tr("Parameter estimation is now available");
}

void MainWindow::loadSceneFromFile(const QString &filename)
{

    SchemaConfig Config;
    //QFile file(qApp->applicationDirPath() + "/SchemaConfig.xml");
    QFile file(filename);

    SchemaScene* scene = Config.deserializeScene(&file, this);

    this->graphicsView->setScene(scene);

    if(m_scene)
        m_scene->deleteLater();

    m_scene = scene;

    if(m_isStarted)
        EventLog << tr("Loaded the Schema file: %1").arg(filename);
    else
        EventLog << tr("Loaded the default Schema");

    if(thread){
        thread->exit();
        EventLog << tr("Active playback was terminated, please, load the playback data again");
    }
}

void MainWindow::loadSimDataFromFile(const QString &filename)
{
    QFileInfo fileInfo(filename);

    QString pbParams = fileInfo.fileName().split(".").first();
//  Extract the Flowrate from Filename (Temp KOCTIb/|b)
    int pbNumCascade = pbParams.split("-").at(1).toInt();
    int pbFlowrate = pbParams.split("-").at(2).toInt();

//  the transcoded filename container must not go out of scope
//  while it is still referenced by char* fileName
//  QByteArray encodedFileName = QfileName.toUtf8();
    QByteArray encodedFileName = filename.toLocal8Bit();
    char* fileName = encodedFileName.data();

    struct st_row::st_row_data* row;
    xlsWorkBook* pWB = xls_open(
                fileName, (char*) "iso-8859-15//TRANSLIT");

//  process workbook if found
    if (pWB != NULL)
    {
        int i;
        int *iDataWorkSheet;
        iDataWorkSheet=NULL;

        for(i=0; i<pWB->sheets.count; i++){
            qDebug() << pWB->sheets.sheet[i].name;
            if(strcmp(pWB->sheets.sheet[i].name,"Tabelle1") == 0) {
                iDataWorkSheet= new int(i);
            }

        }

        if(iDataWorkSheet != NULL) {
            xlsWorkSheet* pWS = xls_getWorkSheet(pWB,*iDataWorkSheet);
            xls_parseWorkSheet(pWS);
            if(Control) Control->deleteLater();
            Control = new PFDControl();

            // Reading the playback xls file
            int t=8; int tt=2;
            do {
                row=&pWS->rows.row[t];

                // Put the data into Control
                if(row->cells.cell[1].d != 0.0)
                    Control->addPoint(row->cells.cell[1].d, row->cells.cell[2].d);

                t++;
            }
            while(row->cells.cell[tt].id==0x27e
                  || row->cells.cell[tt].id==0x0BD
                  || row->cells.cell[tt].id==0x203);


            Control->setPlaybackFlowrate(static_cast<qreal> (pbFlowrate));
            Control->setNumCascade(pbNumCascade);
            Control->setPlaybackFileName(fileInfo.fileName());
            initControl();

            EventLog << tr("Opened the playback file: %1").arg(filename)
                     << tr("Please, set the volume flowrate %1 L/hr"
                               " to begin the simulation playback").arg(pbFlowrate);


            thread->start();
        }
    }
    else
    {
        perror("File open failure");
        QMessageBox::warning(this, tr("File open failure"),
                             tr("Failed to open %1").arg(filename));
    }
}
void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(printAct);
    fileToolBar->setProperty("objectName", QString("fileToolBar"));

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(undoAct);
    editToolBar->setProperty("objectName", QString("editToolBar"));
}
void MainWindow::newFile()
{
/*
    SchemaView* graphicsViewNew = new SchemaView();
    QGraphicsScene* m_sceneNew = new QGraphicsScene();
    graphicsViewNew->setScene(m_sceneNew);
    graphicsViewNew->viewport()->installEventFilter(this);
    graphicsViewNew->setRenderHint(QPainter::Antialiasing);

    this->setCentralWidget(graphicsViewNew);

    */
    //this->adjustSize();
    loadSceneFromFile();
}

void MainWindow::open()
{

    QString fileName = QFileDialog::getOpenFileName(
                this, tr("Open File"), "",
                tr("Reactors Lab files (*.xls *.xml);;"
                   "Simulation playback files (*.xls);;"
                   "Schema files (*.xml)"));
    if (fileName.isNull()) return;
    QFileInfo FileInfo(fileName);
    if(FileInfo.suffix() == "xls")
        loadSimDataFromFile(fileName);
    else if(FileInfo.suffix() == "xml")
        loadSceneFromFile(fileName);


}

void MainWindow::save()
{
    SchemaConfig Config;
    QString fileName = QFileDialog::getSaveFileName(
                this, tr("Save File As"), "",
                tr("Schema files (*.xml)"));
    if (fileName.isNull()) return;
    QFile file(fileName);
    foreach(SchemaItem* item, m_scene->schemaItems()) {
        Config.serializeObject(item, &file);
    }
}

void MainWindow::print()
{
    QPrinter printer;

    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle(tr("Print Document"));
  /*  if (this->textCursor().hasSelection())
        dialog.addEnabledOption(QAbstractPrintDialog::PrintSelection);*/
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QPainter painter;
    painter.begin(&printer);
    double xscale = printer.pageRect().width()/double(graphicsView->width());
    double yscale = printer.pageRect().height()/double(graphicsView->height());
    double scale = qMin(xscale, yscale);
    painter.translate(printer.paperRect().x() + printer.pageRect().width()/2,
                       printer.paperRect().y() + printer.pageRect().height()/2);
    painter.scale(scale, scale);
    painter.translate(-width()/2, -height()/2);

    graphicsView->render(&painter);
    painter.end();
}

void MainWindow::undo()
{

}

void MainWindow::redo()
{

}

void MainWindow::cut()
{

}

void MainWindow::copy()
{

}

void MainWindow::paste()
{

}

void MainWindow::bold()
{

}

void MainWindow::italic()
{

}

void MainWindow::leftAlign()
{

}

void MainWindow::rightAlign()
{

}

void MainWindow::justify()
{

}

void MainWindow::center()
{

}

void MainWindow::setLineSpacing()
{

}

void MainWindow::setParagraphSpacing()
{

}

void MainWindow::about()
{
  //  infoLabel->setText(tr("Invoked <b>Help|About</b>"));
    QMessageBox::about(this, tr("About Reactors Lab"),
            tr("<h3>Reactors Lab</h3><br>Remote chemical engineering lab software<br>"
               "Copyright (C) 2015 St. Petersburg State Institute of Technology & TU Dresden<br><br>"
               "Project supervisor: Prof. Prof. h.c. Dr. rer. nat. habil. Wladimir Reschetilowski<br>"
               "Developers: Dr. Ekaterina Borovinskaya, Dr. Roman Kulishenko<br><br>"
               "This project uses <a href=\"http://www.gnu.org/software/gsl/\">GSL</a>,"
               " <a href=\"http://libxls.sourceforge.net/\">libxls</a>"
               " and <a href=\"http://qcustomplot.com\">QCustomPlot</a> libraries licensed under GNU GPL"));
}

void MainWindow::aboutQt()
{

}

void MainWindow::updateWidgets()
{
    qreal TimeNow = Control->getCurrentTime();
    if(TimeNow == 0.0f) return;

//  TODO: Fix repeated addition of the same point (fixed?)
    int i = 0;

    while ((fabs(TimeNow - Control->getTimeAt(i)) > 0.5 ) && i < Control->getCount() - 1)
    i++;
    qDebug() << i;

    if(i == Control->getCount() - 1) return;

    plotWidget->graph(0)->addData(Control->getTimeAt(i), Control->getParameterAt(i));
    plotWidget->replot();
    qDebug() << "TimeNow:" << QString::number(TimeNow)<< "Added point t = " << QString::number(Control->getTimeAt(i));


    tableWidget->setRowCount(i + 1);

    QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(Control->getTimeAt(i)));
    tableWidget->setItem(i, 0, newItem);

    newItem = new QTableWidgetItem(tr("%1").arg(Control->getParameterAt(i)));
    tableWidget->setItem(i, 1, newItem);
    tableWidget->scrollToBottom();
    tableWidget->selectRow(i);


}

void MainWindow::Run()
{

    Control->Start();
    //MediaPlayer->play();

    EventLog << tr("Simulation playback started");
}

void MainWindow::zoomIn(){
    graphicsView->scale(1.25,1.25);
}
void MainWindow::zoomOut(){
    graphicsView->scale(0.8,0.8);
}

void MainWindow::fitInView()
{
    graphicsView->fitInView(m_scene->sceneRect(),Qt::KeepAspectRatio);
}
void MainWindow::paramEstimation(){
    SchemaData *Data = new SchemaData(Control);
    Data->calcConc();   
    Data->SmoothData();

    Data->calcDimTime();

    Data->calcDimConc();
    Data->calcM2theta();

    ModelCell *Model = new ModelCell(Data);

    EventLog << tr("Started parameter estimation and simulation");

    Model->EstimateNumCells();
    Model->Sim();
    Model->SimODE();

    EventLog << tr("Cell model parameters have been estimated, see the results...");
    QWidget *wnd = new QWidget(this, Qt::Dialog);
    QVBoxLayout* layout = new QVBoxLayout;
    QCustomPlot *resPlotWidget = new QCustomPlot(wnd);
    QLabel* SimParams = new QLabel(wnd);
    SimParams->setText(tr("<h3>Model Summary</h3><br>"
                          "Data Smoothing: Median Filter, Low-Pass Filter<br>"
                          "Paramethers Estimation: Levenberg-Marquardt Method<br>"
                          "Simulation: Exact ODE Solution, Numeric ODE solution"));


    QLabel* SimResults = new QLabel(wnd);
    SimResults->setText(tr("N = %1 (rounded to: %2), Cin = %3 mol/L<br>"
                           "tau = %4 sec, avg_tau = %5 sec<br>"
                           "Method 1: N = %6").arg(QString::number(Model->getNum()),
                                         QString::number(Model->getiNum()),
                                         QString::number(Model->getCin()),
                                         QString::number(Data->getTau()),
                                         QString::number(Data->getAvgTau()),
                                         QString::number(Data->getNc())));

    const QVector<qreal> &SConc = Data->getSConc();
    const QVector<qreal> &DimTime = Data->getDimTime();

    resPlotWidget->addGraph();
    for(int i = 0; i < DimTime.size(); i++) {
        resPlotWidget->graph(0)->addData(Data->getDimTimeAt(i),  Data->getConcAt(i));
        resPlotWidget->graph(0)->setName(tr("Experiment"));
    }

    resPlotWidget->addGraph();
    resPlotWidget->graph(1)->setData(DimTime,  SConc);
    resPlotWidget->graph(1)->setPen(QPen(Qt::red));
    resPlotWidget->graph(1)->setName(tr("Experiment (smoothed)"));
    Qt::GlobalColor Colors[2] = {Qt::green, Qt::magenta };

    for(size_t i = 0; i < Data->getSimConcCount(); i++) {
       const QVector<qreal> &SimConc = Data->getSimConc(i);
       resPlotWidget->addGraph();
       resPlotWidget->graph(i+2)->setData(DimTime, SimConc);
       resPlotWidget->graph(i+2)->setPen(QPen(Colors[i]));
       resPlotWidget->graph(i+2)->setName(tr("Simulated by method %1").arg(i+1));
    }

    resPlotWidget->xAxis->setLabel(tr("Dimensionless Time"));
    resPlotWidget->yAxis->setLabel(tr("Tracer Concentration, mol/L"));
    bool skipExpDataInScale = false;

    if(skipExpDataInScale) {
        resPlotWidget->graph(0)->setVisible(false);
        resPlotWidget->rescaleAxes(true);
        resPlotWidget->graph(0)->setVisible(true);
    } else
        resPlotWidget->rescaleAxes();

    resPlotWidget->setMinimumWidth(600);
    resPlotWidget->setMinimumHeight(400);
    resPlotWidget->legend->setVisible(true);
    resPlotWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    resPlotWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    wnd->setLayout(layout);
    layout->addWidget(resPlotWidget);
    layout->addWidget(SimParams);
        layout->addWidget(SimResults);

    FormulaView* formulaView = new FormulaView(wnd);
    QFile FormulaFile(":/resources/cellmodel.mml");
    if (FormulaFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&FormulaFile);
        QString FormulaString = in.readAll();
        formulaView->setFormula(FormulaString.arg(tr("Exact solution:"),
                                                  tr("Average residence time:"),
                                                  QString::number(Data->getAvgTau(),'f', 3),
                                                  tr("Estimated number of cells:"),
                                                  QString::number(Model->getNum(),'f', 3),
                                                  QString::number(Model->getiNum()),
                                                  tr("Initial tracer concentration:"),
                                                  QString::number(Model->getCin(),'f', 3)));
        formulaView->setMinimumSize(200,300);
        formulaView->setFontSize(12);
        formulaView->setColors(false);

        layout->addWidget(formulaView);
        FormulaFile.close();
    }

    wnd->setWindowTitle(tr("Cell Model (method 2) simulation results"));

    QDesktopWidget *desktop = QApplication::desktop();

    wnd->show();
    wnd->move((desktop->width() - wnd->width())/2,(desktop->height() - wnd->height())/2);
}

void MainWindow::importFromServerDlg()
{

    database = new SchemaDB;

    if(database->getLabsTable()) {

        QWidget *wnd = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout;
        QTableView *view = new QTableView;

        database->setData(Control);

        view->setModel(database->getLabsModel());
        view->hideColumn(0);
        view->hideColumn(1);
        view->setEditTriggers(QTableView::NoEditTriggers);
        view->setSelectionBehavior(QTableView::SelectRows);
        wnd->setLayout(layout);
        layout->addWidget(view);

        wnd->setWindowTitle(tr("Select the case study"));
        wnd->resize(view->width(), view->height());
        wnd->show();

        connect(view, SIGNAL(doubleClicked(QModelIndex)), database, SLOT(getLabID(QModelIndex)));
        connect(database, SIGNAL(getLabDataFinishedResult(bool)), this, SLOT(importFromServer(bool)));
        connect(database, SIGNAL(getLabDataFinished()), wnd, SLOT(close()));
    }

}

void MainWindow::importFromServer(bool result)
{
    if(result) {
        Control = database->getData();
        database->deleteLater();
        initControl();

        EventLog << tr("Imported the playback data from server")
                 << tr("Please, set the volume flowrate %1 L/hr in order to begin "
                       "the simulation playback").arg(Control->getPlaybackFlowrate());

        thread->start();
    } else
        QMessageBox::warning(this, tr("Couldn't import the data"),tr("Couldn't import the data"));
}

void MainWindow::exportToServer()
{
    SchemaDB* worker = new SchemaDB;
    QThread* thread = new QThread;
    worker->setData(Control);
    worker->moveToThread(thread);

    connect(thread, SIGNAL(started()), worker, SLOT(sendLabData()));
    connect(worker, SIGNAL(finishedResult(bool)), this, SLOT(exportFinished(bool)));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));

    exportToServerAct->setDisabled(true);
    thread->start();

}

void MainWindow::loadSettings()
{
    QSettings settings("SPbSIT", "ReactosLab");
    settings.beginGroup("MainWindow");

    if(settings.contains("geometry"))
        restoreGeometry(settings.value("geometry", QByteArray()).toByteArray());
    if(settings.contains("state"))
        restoreState(settings.value("state", QByteArray()).toByteArray());
    if(settings.contains("view_scale")){
        QPointF view_scale = settings.value("view_scale").toPointF();
        if (view_scale.x() > 0.25 && view_scale.y() > 0.25)
            graphicsView->scale(view_scale.x(), view_scale.y());
    }
    else
        graphicsView->fitInView(m_scene->sceneRect(),Qt::KeepAspectRatio);
    settings.endGroup();
}

void MainWindow::saveSettings()
{
    QSettings settings("SPbSIT", "ReactosLab");
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.setValue("view_scale", QPointF(graphicsView->transform().m11(), graphicsView->transform().m22()));
    settings.endGroup();    
    settings.sync();
}

void MainWindow::exportFinished(bool result) {
    if (result) {
        QMessageBox::information(this,tr("Export successful"),
                                 tr("Export has been successfuly finished"),QMessageBox::Ok);
        EventLog << tr("Exported the lab data to remote server");
        }
    else
        QMessageBox::warning(this, tr("Couldn't export the data"), tr("Couldn't export the data"));
    exportToServerAct->setEnabled(true);
}

void MainWindow::onlineMode()
{
    //m_RunMode = RunMode::Online;
    SchemaItem::SchemaMode = SchemaItem::RunMode::Online;
}

void MainWindow::offlineMode()
{
    //m_RunMode = RunMode::Offline;
    SchemaItem::SchemaMode = SchemaItem::RunMode::Offline;

}

void MainWindow::editMode()
{
    //m_RunMode = RunMode::Edit;
    SchemaItem::SchemaMode = SchemaItem::RunMode::Edit;
}
