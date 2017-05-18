#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_stockCanvas = new StockCanvas(this);
    m_stockCanvas->setObjectName(tr("stock_time"));
    setCentralWidget(m_stockCanvas);



    resize(800,600);
}

MainWindow::~MainWindow()
{
    delete ui;
}
