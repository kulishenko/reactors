
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
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createMenus();
    createToolBars();
    createDockWindows();
    createSchemaView();

    QString message = tr("A context menu is available by right-clicking");
    statusBar()->showMessage(message);

    setWindowTitle(tr("Reactors Lab"));
    setMinimumSize(50, 50);

    this->setCentralWidget(graphicsView);
    this->adjustSize();

    loadSettings();

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
 //   connect(playbackAct, SIGNAL(triggered()), this, SLOT(about()));

    onlineAct = new QAction(tr("&Online"),this);
    onlineAct->setCheckable(true);
    onlineAct->setStatusTip(tr("Enable the online mode and connect to remote Lab server"));
 //   connect(playbackAct, SIGNAL(triggered()), this, SLOT(about()));

    modeGroup = new QActionGroup(this);
    modeGroup->addAction(playbackAct);
    modeGroup->addAction(onlineAct);
    playbackAct->setChecked(true);
    onlineAct->setDisabled(true);

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

    dataMenu = menuBar()->addMenu(tr("&Data"));
    dataMenu->addAction(importFromServerAct);
    dataMenu->addAction(exportToServerAct);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(paramEstimAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    formatMenu = editMenu->addMenu(tr("&Format"));
    formatMenu->addAction(boldAct);
    formatMenu->addAction(italicAct);
    formatMenu->addSeparator()->setText(tr("Alignment"));
    formatMenu->addAction(leftAlignAct);
    formatMenu->addAction(rightAlignAct);
    formatMenu->addAction(justifyAct);
    formatMenu->addAction(centerAct);
    formatMenu->addSeparator();
    formatMenu->addAction(setLineSpacingAct);
    formatMenu->addAction(setParagraphSpacingAct);
}

void MainWindow::createDockWindows()
{


    QDockWidget *dock = new QDockWidget(tr("Video"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    videoWidget = new QVideoWidget(dock);
    MediaPlayer = new QMediaPlayer(this);
    QMediaPlaylist* playlist = new QMediaPlaylist(this);
    //playlist->addMedia(QUrl::fromLocalFile("C:/Users/Public/Videos/Sample Videos/Wildlife.wmv"));
    playlist->addMedia(QUrl::fromLocalFile("C:/Users/Roman/Documents/Reactors/GERMAN_DEUTSCH.m3u"));
    playlist->setCurrentIndex(1);
    MediaPlayer->setPlaylist(playlist);
    MediaPlayer->setVideoOutput(videoWidget);
    dock->setWidget(videoWidget);
    dock->setMinimumHeight(150);

    addDockWidget(Qt::RightDockWidgetArea, dock);

    dock = new QDockWidget(tr("Plot"), this);

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
    tableWidget = new QTableWidget(1,2,dock);

    QPalette* palette = new QPalette();
    palette->setColor(QPalette::Highlight,Qt::green);
    tableWidget->setPalette(*palette);

    tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Time, s") << tr("Conductivity, mS/cm"));
    tableWidget->horizontalHeader()->setStretchLastSection(true);

    dock->setWidget(tableWidget);
    addDockWidget(Qt::RightDockWidgetArea,dock);

    dock = new QDockWidget(tr("Event Log"), this);
    eventsWidget = new QListWidget(dock);
    dock->setWidget(eventsWidget);
    dock->setMaximumHeight(100);
    addDockWidget(Qt::BottomDockWidgetArea, dock);

    EventLog.setWidget(eventsWidget);
    EventLog  << tr("Program started");
}

void MainWindow::createSchemaView()
{
    graphicsView = new SchemaView();

    bgColor = QColor::fromRgb(240, 240, 240);

    m_scene = new QGraphicsScene();
//  m_scene->setBackgroundBrush(QBrush(QColor::fromRgb(0, 150, 140), Qt::SolidPattern));
//  m_scene->setBackgroundBrush(QBrush(bgColor, Qt::SolidPattern));

    SchemaStream* streamItem1 = new SchemaStream(50,0, 400, 0);
    m_scene->addItem(streamItem1);

    SchemaStream* streamItem2 = new SchemaStream(50,1200, 650, 90);
    m_scene->addItem(streamItem2);

    // Temp
    SchemaCell* MeasurementCell = new SchemaCell(100,40,1255, 550);

    m_scene->addItem(MeasurementCell);

    valveItem1 = new SchemaValve(30,45,122.5,350,-90);
    m_scene->addItem(valveItem1);

    // Creating CSTR Items
    for(int i=1; i<=5; i++)
        reactorItems.push_back(new SchemaCSTR(120,90,i*200,i*70,0.1,i-1));

    ModelCSTR* CSTR = new ModelCSTR();
    CSTR->setProperty("Level", 0.1);


    flowmeterItem = new SchemaFlowmeter(25,200,125,50,0);

    m_scene->addItem(flowmeterItem);

    // Connecting Items
    SchemaPipeline* Line000 = new SchemaPipeline(streamItem1,valveItem1);
    SchemaPipeline* Line00 = new SchemaPipeline(valveItem1,flowmeterItem);

    valveItem1->Descedant = flowmeterItem;

    // Connecting CSTRs

    pipelineItems.push_back(new SchemaPipeline(flowmeterItem,reactorItems.at(0)));
    for(int i=0;i<reactorItems.size()-1;i++)
        pipelineItems.push_back(new SchemaPipeline(reactorItems.at(i),reactorItems.at(i+1)));


    pipelineItems.push_back(new SchemaPipeline(reactorItems.last(),streamItem2));
    m_scene->addItem(Line000);
    m_scene->addItem(Line00);


    for(int i=0;i<reactorItems.size();i++)
        m_scene->addItem(reactorItems.at(i));

    for(int i=0;i<pipelineItems.size();i++)
        m_scene->addItem(pipelineItems.at(i));

    graphicsView->setScene(m_scene);
    graphicsView->viewport()->installEventFilter(this);
    graphicsView->setRenderHint(QPainter::Antialiasing);

}

void MainWindow::initControl()
{

    Control->reactorItems = &reactorItems;

    for(int i=0; i<reactorItems.size();i++)
        Control->addItem(reactorItems.at(i));

    Control->addItem(flowmeterItem);
    Control->calcTau();

    thread = new QThread(this);
    timer = new QTimer();
    timer->setInterval(1000);
    timer->moveToThread(thread);
    Control->moveToThread(thread);

    connect(thread, SIGNAL(started()), timer, SLOT(start()));
    connect(timer, SIGNAL(timeout()), Control, SLOT(tick()));

    connect(this, SIGNAL(destroyed()), thread, SLOT(quit()));

    connect(valveItem1, SIGNAL(increase()),Control,SLOT(flowrate_increase()));
    connect(valveItem1, SIGNAL(decrease()),Control,SLOT(flowrate_decrease()));


    connect(Control, SIGNAL(setLevel()),reactorItems.at(0),SLOT(fill()));

    connect(Control, SIGNAL(doSim()),this,SLOT(updateWidgets()));
    connect(Control, SIGNAL(startSim()),this,SLOT(Run()));

    EventLog << tr("Schema controls are initialized");

    plotWidget->graph(0)->setData(Control->Time, Control->Conductivity);
    plotWidget->rescaleAxes();
    plotWidget->replot();
    plotWidget->graph(0)->clearData();
    plotWidget->replot();

    // ToDo: Should be activated at the end of experiment
    paramEstimAct->setDisabled(false);
    exportToServerAct->setDisabled(false);
    EventLog << tr("Parameter estimation is now available");
}
void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(printAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(undoAct);
}
void MainWindow::newFile()
{

    QGraphicsView* graphicsViewNew = new SchemaView();
    QGraphicsScene* m_sceneNew = new QGraphicsScene();
    graphicsViewNew->setScene(m_sceneNew);
    graphicsViewNew->viewport()->installEventFilter(this);
    graphicsViewNew->setRenderHint(QPainter::Antialiasing);

    this->setCentralWidget(graphicsViewNew);
    //this->adjustSize();
}

void MainWindow::open()
{

    QString QfileName = QFileDialog::getOpenFileName(
                this, tr("Open File"), "",
                tr("Simulation Playback files (*.xls)"));
    if (QfileName.isNull()) return;
    QFileInfo fileInfo(QfileName);

    QString pbParams = fileInfo.fileName().split(".").first();
//  Extract the Flowrate from Filename (Temp KOCTIb/|b)
    int pbNumCascade = pbParams.split("-").at(1).toInt();
    int pbFlowrate = pbParams.split("-").at(2).toInt();

//  the transcoded filename container must not go out of scope
//  while it is still referenced by char* fileName
//  QByteArray encodedFileName = QfileName.toUtf8();
    QByteArray encodedFileName = QfileName.toLocal8Bit();
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
            Control = new PFDControl();

            // Reading the playback xls file
            int t=8; int tt=2;
            do {
                row=&pWS->rows.row[t];

                // Put the data into Control
                Control->Time.push_back(row->cells.cell[1].d);
                Control->Conductivity.push_back(row->cells.cell[2].d);

                t++;
            }
            while(row->cells.cell[tt].id==0x27e
                  || row->cells.cell[tt].id==0x0BD
                  || row->cells.cell[tt].id==0x203);


            Control->setPlaybackFlowrate(static_cast<qreal> (pbFlowrate));
            Control->setNumCascade(pbNumCascade);
            Control->PlaybackFileName = fileInfo.fileName();
            initControl();

            EventLog << tr("Opened the playback file: %1").arg(QfileName)
                     << tr("Please, set the volume flowrate %1 L/hr"
                               " to begin the simulation playback").arg(pbFlowrate);


            thread->start();
        }
    }
    else
    {
        perror("File open failure");
        QMessageBox::warning(this, tr("File open failure"),
                             tr("Failed to open %1").arg(QfileName));
    }
}

void MainWindow::save()
{

}

void MainWindow::print()
{

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
    if(Control->TimeNow==0.0f) return;

//  TODO: Fix repeated addition of the same point
    int i = 0;
    do i++;
    while ((fabs(Control->TimeNow - Control->Time.at(i)) > 0.5 ) && i < Control->Time.size()-1);

    if(i==Control->Time.size()-1) return;

    plotWidget->graph(0)->addData(Control->Time.at(i), Control->Conductivity.at(i));
    plotWidget->replot();
    qDebug() << "TimeNow:" <<QString::number(Control->TimeNow)<< "Added point t = " << QString::number(Control->Time.at(i));


    tableWidget->setRowCount(i+1);

    QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(Control->Time.at(i)));
    tableWidget->setItem(i, 0, newItem);

    newItem = new QTableWidgetItem(tr("%1").arg(Control->Conductivity.at(i)));
    tableWidget->setItem(i, 1, newItem);
    tableWidget->scrollToBottom();
    tableWidget->selectRow(i);


}

void MainWindow::Run()
{

    Control->Start();
    MediaPlayer->play();

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
    Data = new SchemaData(Control);
    Data->calcConc();   
    Data->SmoothData();
    Data->calcAvgTau();
    Data->calcDimTime();

    Data->calcDimConc();
    Data->calcM2t();

    ModelCell *Model = new ModelCell(Data);

    Model->EstimateNumCells();
    Model->Sim();
    Model->SimODE();

    QWidget *wnd = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout;
    QCustomPlot *resPlotWidget = new QCustomPlot();
    QLabel* SimResults = new QLabel;
    SimResults->setText(tr("N = %1 (rounded to: %2), Cin = %3 mol/L<br>"
                           "tau = %4 sec, avg_tau = %5 sec<br>"
                           "N = %6").arg(QString::number(Model->Num),
                                         QString::number(Model->iNum),
                                         QString::number(Model->Cin),
                                         QString::number(Data->tau),
                                         QString::number(Data->avg_tau),
                                         QString::number(Data->Nc)));


    resPlotWidget->addGraph();
    resPlotWidget->graph(0)->setData(Data->DimTime,  Data->Conc);
    resPlotWidget->graph(0)->setName("Experiment");

    resPlotWidget->addGraph();
    resPlotWidget->graph(1)->setData(Data->DimTime,  Data->SConc);
    resPlotWidget->graph(1)->setPen(QPen(Qt::red));
    resPlotWidget->graph(1)->setName("Experiment (smoothed)");
    Qt::GlobalColor Colors[2] = {Qt::green, Qt::magenta };

    for(int i = 0; i < Data->SimConc.size(); i++) {
       resPlotWidget->addGraph();
       resPlotWidget->graph(i+2)->setData(Data->DimTime,  *Data->SimConc.at(i));
       resPlotWidget->graph(i+2)->setPen(QPen(Colors[i]));
       resPlotWidget->graph(i+2)->setName(tr("Simulated by method %1").arg(i+1));
    }

    resPlotWidget->xAxis->setLabel(tr("Dimensionless Time"));
    resPlotWidget->yAxis->setLabel(tr("Tracer Concentration, mol/L"));

    resPlotWidget->setMinimumWidth(600);
    resPlotWidget->setMinimumHeight(400);
    resPlotWidget->legend->setVisible(true);
    resPlotWidget->rescaleAxes();
    resPlotWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    resPlotWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    wnd->setLayout(layout);
    layout->addWidget(resPlotWidget);
    layout->addWidget(SimResults);
    wnd->setWindowTitle(tr("Cell Model (method 2) simulation results"));
    wnd->show();

}

void MainWindow::importFromServerDlg()
{

    database = new SchemaDB;

    if(database->getLabsTable()) {

        QWidget *wnd = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout;
        QTableView *view = new QTableView;

        database->setData(Control);
        view->setModel(database->LabsModel);
        view->hideColumn(0);
        view->hideColumn(1);
        view->setEditTriggers(QTableView::NoEditTriggers);
        view->setSelectionBehavior(QTableView::SelectRows);
        wnd->setLayout(layout);
        layout->addWidget(view);

        wnd->setWindowTitle(tr("Select the case study"));
        wnd->resize(view->width(),view->height());
        wnd->show();

        connect(view,SIGNAL(doubleClicked(QModelIndex)),database,SLOT(getLabID(QModelIndex)));
        connect(database,SIGNAL(getLabDataFinishedResult(bool)),this,SLOT(importFromServer(bool)));
        connect(database,SIGNAL(getLabDataFinished()),wnd,SLOT(close()));
    }

}

void MainWindow::importFromServer(bool result)
{
    if(result) {
        Control = database->getData();
        delete database;
        initControl();

        EventLog << tr("Imported the playback data from server")
                 << tr("Please, set the volume flowrate %1 L/hr in order to begin "
                            "the simulation playback").arg(Control->PlaybackFlowrate);

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
    bool maximized;
    if(settings.contains("maximized")){
        maximized = settings.value("maximized").toBool();
        setWindowState(Qt::WindowMaximized);
    }
    if(settings.contains("size") && !maximized){
        QSize size = settings.value("size").toSize();
        resize(size);
    }
    if(settings.contains("pos")){
        QPoint pos = settings.value("pos").toPoint();
        move(pos);
    }
    if(settings.contains("view_scale_X") && settings.contains("view_scale_Y")){
        qreal sx = settings.value("view_scale_X").toDouble();
        qreal sy = settings.value("view_scale_Y").toDouble();
        graphicsView->scale(sx,sy);
    }
    else
        graphicsView->fitInView(m_scene->sceneRect(),Qt::KeepAspectRatio);
    settings.endGroup();
}

void MainWindow::saveSettings()
{
    QSettings settings("SPbSIT", "ReactosLab");
    settings.beginGroup("MainWindow");
    settings.setValue("maximized", isMaximized());
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("view_scale_X", graphicsView->transform().m11());
    settings.setValue("view_scale_Y", graphicsView->transform().m22());
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
        QMessageBox::warning(this, tr("Couldn't export the data"),tr("Couldn't export the data"));
    exportToServerAct->setEnabled(true);
}
