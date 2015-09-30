
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QEvent>
#include <QResizeEvent>
#include <QMessageBox>
#include <QDockWidget>
#include <QVideoSurfaceFormat>
#include <QGraphicsVideoItem>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
   ui(new Ui::MainWindow)
{
  ui->setupUi(this);
 //   QGraphicsScene* scene = new QGraphicsScene(this);
 //   ui->graphicsView->setScene(scene);

    createActions();
    createMenus();
    createToolBars();
    createDockWindows();

    QString message = tr("A context menu is available by right-clicking");
    statusBar()->showMessage(message);

    setWindowTitle(tr("Reactors Lab"));
    setMinimumSize(50, 50);


    graphicsView = new QGraphicsView();


    m_scene = new QGraphicsScene();

    SchemaStream* streamItem1 = new SchemaStream(50,0, 400, 0);
    m_scene->addItem(streamItem1);

    SchemaStream* streamItem2 = new SchemaStream(50,1200, 650, 90);
    m_scene->addItem(streamItem2);

    valveItem1 = new SchemaValve(30,45,122.5,350,-90);
    m_scene->addItem(valveItem1);

    //Creating CSTR Items
    for(int i=1; i<=5; i++)
        reactorItems.push_back(new SchemaVessel(120,90,i*200,i*100,0.1));


    SchemaFlowmeter* Flowmeter1 = new SchemaFlowmeter(25,200,125,50,0);

    m_scene->addItem(Flowmeter1);

    // Connecting Items
    SchemaPipeline* Line000 = new SchemaPipeline(streamItem1,valveItem1);
    SchemaPipeline* Line00 = new SchemaPipeline(valveItem1,Flowmeter1);

    valveItem1->Descedant = Flowmeter1;

    // Connecting CSTRs

    pipelineItems.push_back(new SchemaPipeline(Flowmeter1,reactorItems.at(0)));
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

    this->setCentralWidget(graphicsView);
    this->adjustSize();

}

MainWindow::~MainWindow()
{
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

    // create plot (from quadratic plot example):
    QVector<double> x(101), y(101);
    for (int i=0; i<101; ++i)
    {
    //  x[i] = i/50.0 - 1;
      x[i] = i;
      y[i] = x[i]*x[i];
    }
    plotWidget->addGraph();
    plotWidget->graph(0)->setData(x, y);
    plotWidget->xAxis->setLabel(tr("Time, s"));
    plotWidget->yAxis->setLabel(tr("Conductivity, mS/cm"));
    plotWidget->rescaleAxes();
    dock->setWidget(plotWidget);
    dock->setMinimumWidth(400);
    dock->setMaximumHeight(400);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    dock = new QDockWidget(tr("Table"), this);
    tableWidget = new QTableWidget(100,2,dock);

    QPalette* palette = new QPalette();
    palette->setColor(QPalette::Highlight,Qt::green);
    tableWidget->setPalette(*palette);

    tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Time, s") << tr("Conductivity, mS/cm"));
    for(int row=0; row!=tableWidget->rowCount(); ++row){
        for(int column=0; column!=tableWidget->columnCount(); ++column) {
            QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg((column==0?x[row]:y[row])));
            tableWidget->setItem(row, column, newItem);
        }
    }

    dock->setWidget(tableWidget);
    addDockWidget(Qt::RightDockWidgetArea,dock);

    dock = new QDockWidget(tr("Event Log"), this);
    eventsWidget = new QListWidget(dock);
    QDateTime EventTime(QDateTime::currentDateTime());
    eventsWidget->addItems(QStringList() << EventTime.toString("[hh:mm:ss.zzz]: ") + tr("Program started"));
    dock->setWidget(eventsWidget);
    dock->setMaximumHeight(100);
    addDockWidget(Qt::BottomDockWidgetArea, dock);


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

     QGraphicsView* graphicsViewNew = new QGraphicsView();
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
    char* fileName = QfileName.toLatin1().data();

    struct st_row::st_row_data* row;
    xlsWorkBook* pWB = xls_open(
                fileName, (char*) "iso-8859-15//TRANSLIT");

    // process workbook if found
    if (pWB != NULL)
    {
        int i;
        int iDataWorkSheet;

        //     bool isParsed;
        for(i=0; i<pWB->sheets.count; i++){
            qDebug() << pWB->sheets.sheet[i].name;
            if(strcmp(pWB->sheets.sheet[i].name,"Tabelle1") == 0) {
                iDataWorkSheet=i;
            }

        }
        if(iDataWorkSheet != 0) {
            xlsWorkSheet* pWS = xls_getWorkSheet(pWB,iDataWorkSheet);
            xls_parseWorkSheet(pWS);
            Control = new PFDControl();

            // Reading the playback xls file
            int t=8; int tt=2;
            do {
                row=&pWS->rows.row[t];
                QTableWidgetItem *newItem = new QTableWidgetItem(
                            tr("%1").arg(row->cells.cell[1].d));
                // Put the data into Control
                Control->Time.push_back(row->cells.cell[1].d);

                tableWidget->setItem(t-8, 0, newItem);

                newItem = new QTableWidgetItem(
                            tr("%1").arg(row->cells.cell[2].d));
                Control->Conductivity.push_back(row->cells.cell[2].d);
                tableWidget->setItem(t-8, 1, newItem);


                tableWidget->setRowCount(t-6);
                t++;
            }
            while(row->cells.cell[tt].id==0x27e
                  || row->cells.cell[tt].id==0x0BD
                  || row->cells.cell[tt].id==0x203);


            thread = new QThread(this);
            timer = new QTimer();
            timer->setInterval(1000);
            timer->moveToThread(thread);
            Control->moveToThread(thread);

            QObject::connect(thread, SIGNAL(started()), timer, SLOT(start()));
            QObject::connect(timer, SIGNAL(timeout()), Control, SLOT(tick()));

            // QObject::connect(reactorItem1, SIGNAL(test()),Control,SLOT(test()));

            QObject::connect(valveItem1, SIGNAL(increase()),Control,SLOT(flowrate_increase()));
            QObject::connect(valveItem1, SIGNAL(decrease()),Control,SLOT(flowrate_decrease()));

            for(int i=0;i<reactorItems.size();i++)
                QObject::connect(Control, SIGNAL(setLevel1()),reactorItems.at(i),SLOT(fill()));

            QObject::connect(Control, SIGNAL(doSim()),this,SLOT(updateWidgets()));
            QObject::connect(Control, SIGNAL(startSim()),this,SLOT(Run()));

            qDebug() << QString::number(Control->Time.size())
                     << " " << QString::number(Control->Conductivity.size());
            qDebug() << QString::number(Control->Time.last());

            plotWidget->graph(0)->setData(Control->Time, Control->Conductivity);
            plotWidget->rescaleAxes();
            plotWidget->replot();
            QDateTime EventTime(QDateTime::currentDateTime());

            eventsWidget->addItems(QStringList() << EventTime.toString("[hh:mm:ss.zzz]: ")
                                   + tr("Opened the playback file: ")+QfileName);
            thread->start();
        }
    }
}

void MainWindow::save()
{
 //   infoLabel->setText(tr("Invoked <b>File|Save</b>"));
}

void MainWindow::print()
{
//    infoLabel->setText(tr("Invoked <b>File|Print</b>"));
}

void MainWindow::undo()
{
 //   infoLabel->setText(tr("Invoked <b>Edit|Undo</b>"));
}

void MainWindow::redo()
{
 //  infoLabel->setText(tr("Invoked <b>Edit|Redo</b>"));
}

void MainWindow::cut()
{
  //  infoLabel->setText(tr("Invoked <b>Edit|Cut</b>"));
}

void MainWindow::copy()
{
   // infoLabel->setText(tr("Invoked <b>Edit|Copy</b>"));
}

void MainWindow::paste()
{
  //  infoLabel->setText(tr("Invoked <b>Edit|Paste</b>"));
}

void MainWindow::bold()
{
  //  infoLabel->setText(tr("Invoked <b>Edit|Format|Bold</b>"));
}

void MainWindow::italic()
{
  //  infoLabel->setText(tr("Invoked <b>Edit|Format|Italic</b>"));
}

void MainWindow::leftAlign()
{
  //  infoLabel->setText(tr("Invoked <b>Edit|Format|Left Align</b>"));
}

void MainWindow::rightAlign()
{
   // infoLabel->setText(tr("Invoked <b>Edit|Format|Right Align</b>"));
}

void MainWindow::justify()
{
  //  infoLabel->setText(tr("Invoked <b>Edit|Format|Justify</b>"));
}

void MainWindow::center()
{
  //  infoLabel->setText(tr("Invoked <b>Edit|Format|Center</b>"));
}

void MainWindow::setLineSpacing()
{
  //  infoLabel->setText(tr("Invoked <b>Edit|Format|Set Line Spacing</b>"));
}

void MainWindow::setParagraphSpacing()
{
  //  infoLabel->setText(tr("Invoked <b>Edit|Format|Set Paragraph Spacing</b>"));
}

void MainWindow::about()
{
  //  infoLabel->setText(tr("Invoked <b>Help|About</b>"));
    QMessageBox::about(this, tr("About Reactors Lab"),
            tr("<h3>Reactors Lab</h3><br>Remote chemical engineering lab software<br>"
               "Copyright (C) 2015 St. Petersburg State Institute of Technology & TU Dresden<br><br>"
               "Project supervisor: Prof. Prof. h.c. Dr. rer. nat. habil. Wladimir Reschetilowski<br>"
               "Developers: Dr. Ekaterina Borovinskaya, Dr. Roman Kulishenko<br><br>"
               "This project uses <a href=\"http://libxls.sourceforge.net/\">libxls</a>"
               " and <a href=\"http://qcustomplot.com\">QCustomPlot</a> libraries licensed under GNU GPL"));
}

void MainWindow::aboutQt()
{
    //   infoLabel->setText(tr("Invoked <b>Help|About Qt</b>"));
}

void MainWindow::updateWidgets()
{
    int i = 0;
    do i++;
    while ((fabs(Control->TimeNow - Control->Time.at(i)) > 1 ) && i <= Control->Time.size());


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
    plotWidget->graph(0)->clearData();
    tableWidget->setRowCount(1);
    Control->Start();
    MediaPlayer->play();
    QDateTime EventTime(QDateTime::currentDateTime());
    eventsWidget->addItems(QStringList() << EventTime.toString("[hh:mm:ss.zzz]: ")+tr("Simulation playback started"));
}


 /*
 bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    if(event->type() == QEvent::Resize )
    {
   //     QResizeEvent *res = reinterpret_cast<QResizeEvent*>(event);
   //     m_elipse->setRect(0, 0, res->size().width(), res->size().height());

        return true;
    }
    if(event->type() == QEvent::MouseButtonPress)
    {
  //  reactorItem1->LiquidLevel=0.9;
 //   reactorItem1->setLevel(0.8);
  //  reactorItem1->Item->update();
  // QMessageBox msgBox;
  // reactorItem1->Item->setBrush( QBrush(Qt::red));
//    msgBox.setText("Changing CSTR1 Level.");
  //   msgBox.exec();reactorItem1->update();

        return true;
    }
    return false;
}
*/
