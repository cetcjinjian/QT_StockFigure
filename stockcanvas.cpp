#include "stockcanvas.h"
#include <QDebug>
#include <QPen>
#include <QPainter>
#include <QDateTime>
#include "qmydata.h"

StockCanvas::StockCanvas(QWidget *parent) : QWidget(parent)
{

    //默认铺满窗口并设置背景色为黑色
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Window,QColor("#000000"));
    this->setPalette(palette);
    //开启鼠标追踪
    setMouseTracking(true);
}


void StockCanvas::paintEvent(QPaintEvent *event)
{
    DrawBK();
    DrawMouseLine();
    DrawFSJL((char*)"000001K",(char*)"20100310");
}

void StockCanvas::resizeEvent(QResizeEvent *event)
{
    m_windowHeight = this->height();
    m_windowWidth = this->width();


}


void StockCanvas::DrawBorder()
{

    //draw outer border

    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FF0000"));
    painter.setPen(pen);


    painter.drawLine(0+BORDER_SIZE,0+BORDER_SIZE,
                     0+BORDER_SIZE,m_windowHeight-BORDER_SIZE); //left |
    painter.drawLine(0+BORDER_SIZE,0+BORDER_SIZE,
                     m_windowWidth-BORDER_SIZE,0+BORDER_SIZE); //top -
    painter.drawLine(m_windowWidth-BORDER_SIZE,0+BORDER_SIZE,
                     m_windowWidth-BORDER_SIZE,m_windowHeight-BORDER_SIZE); //right |
    painter.drawLine(0+BORDER_SIZE,m_windowHeight-BORDER_SIZE,
                     m_windowWidth-BORDER_SIZE,m_windowHeight-BORDER_SIZE); //bottom -

}

void StockCanvas::DrawBK()
{

    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FF0000"));
    painter.setPen(pen);


    //DrawBorder();

    //画背景表格

    painter.drawLine(0+BORDER_SIZE+COORDINATE_X1,0+BORDER_SIZE+COORDINATE_Y1,
                     0+BORDER_SIZE+COORDINATE_X1,m_windowHeight-BORDER_SIZE-COORDINATE_Y2);
    painter.drawLine(0+BORDER_SIZE+COORDINATE_X1,0+BORDER_SIZE+COORDINATE_Y1,
                     m_windowWidth-BORDER_SIZE-COORDINATE_X2,0+BORDER_SIZE+COORDINATE_Y1);
    painter.drawLine(m_windowWidth-BORDER_SIZE-COORDINATE_X2,0+BORDER_SIZE+COORDINATE_Y1,
                     m_windowWidth-BORDER_SIZE-COORDINATE_X2,m_windowHeight-BORDER_SIZE-COORDINATE_Y2);
    painter.drawLine(0+BORDER_SIZE+COORDINATE_X1,m_windowHeight-BORDER_SIZE-COORDINATE_Y2,
                     m_windowWidth-BORDER_SIZE-COORDINATE_X2,m_windowHeight-BORDER_SIZE-COORDINATE_Y2);


    yInterval = (double)( (m_windowHeight-BORDER_SIZE-COORDINATE_Y2)- (0+BORDER_SIZE+COORDINATE_Y1) ) / 15 + 0.5; //三部分，每部分5个大格子
    pen.setColor(QColor("#FF0000"));
    for( int i=1,j=yInterval; i < 15; i++,j+=yInterval)
    {
        if( i % 5 != 0 )
        {
            pen.setStyle(Qt::DashLine);
            painter.setPen(pen);
        }
        else
        {
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
        }
        painter.drawLine(0+BORDER_SIZE+COORDINATE_X1,0+BORDER_SIZE+COORDINATE_Y1+j,
                         m_windowWidth-BORDER_SIZE-COORDINATE_X2,0+BORDER_SIZE+COORDINATE_Y1+j);
    }

    xInterval = (double)( (m_windowWidth-BORDER_SIZE-COORDINATE_X2)- (0+BORDER_SIZE+COORDINATE_X1) ) / 8 + 0.5;
    for( int x=1,y=xInterval; x < 8; x++,y+=xInterval)
    {

        if( x % 2 != 0 )
        {
            pen.setStyle(Qt::DashLine);
            painter.setPen(pen);
        }
        else
        {
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
        }
        painter.setPen(pen);
        painter.drawLine(0+BORDER_SIZE+COORDINATE_X1+y,0+BORDER_SIZE+COORDINATE_Y1,
                         0+BORDER_SIZE+COORDINATE_X1+y,m_windowHeight-BORDER_SIZE-COORDINATE_Y2);
    }
}

void StockCanvas:: mouseMoveEvent(QMouseEvent* event)
{
    mousePoint = QPoint(event->pos());

    m_xGridMin = BORDER_SIZE+COORDINATE_X1;
    m_xGridMax = m_windowWidth - (BORDER_SIZE+COORDINATE_X1);

    m_yGridMin = BORDER_SIZE+COORDINATE_Y1;
    //yInterval  = (double)( (m_windowHeight-BORDER_SIZE-COORDINATE_Y2)- (0+BORDER_SIZE+COORDINATE_Y1) ) / 15 + 0.5;
    //m_yGridMax = m_windowHeight-BORDER_SIZE-COORDINATE_Y2-5*(yInterval);
    m_yGridMax = m_windowHeight-BORDER_SIZE-COORDINATE_Y2;

    if(mousePoint.y() < m_yGridMin || mousePoint.y() > m_yGridMax){
        return;
    }
    if(mousePoint.x() < m_xGridMin || mousePoint.x() > m_xGridMax){
        return;
    }

    update();
}

void StockCanvas::DrawMouseLine()
{
    QLineF line(0+BORDER_SIZE+COORDINATE_X1,mousePoint.y(),this->width()-(BORDER_SIZE+COORDINATE_X1),mousePoint.y());
    QLineF line2(mousePoint.x(),BORDER_SIZE+COORDINATE_Y1,mousePoint.x(),this->height()-(BORDER_SIZE+COORDINATE_Y2));
    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(line);
    painter.drawLine(line2);
    DrawTips();
}



void StockCanvas::DrawFSJL(char* SecID,char* szDate)
{
    QMyData fsjl(SecID,szDate);
    if( !fsjl.ReadFSJL() )
        return;
    fsjl.GetFSJLINFO();


    pervalue = fsjl.info.PerValue;
    price_start = fsjl.info.Start;

    QPainter painter(this);
    QPen     pen;
    pen.setColor(Qt::white);
    painter.setPen(pen);

    //画右侧涨跌百分比 0%
    yInterval  = (double)( (m_windowHeight-BORDER_SIZE-COORDINATE_Y2)- (0+BORDER_SIZE+COORDINATE_Y1) ) / 15 ;
    painter.drawText(QPoint(m_windowWidth-BORDER_SIZE-COORDINATE_X2+10,
                            0+BORDER_SIZE+COORDINATE_Y1+yInterval*5 +2),QString("0.00%"));

    //画左侧初始股票价格 0

    QString str;
    str.sprintf("%.2f",(float)fsjl.info.Start / 1000 );
    painter.drawText(QPoint(0+BORDER_SIZE+COORDINATE_X1-TEXT_LENGTH,
                            0+BORDER_SIZE+COORDINATE_Y1+yInterval*5 +2),str);


    //根据读入的数据画出涨跌百分比和股票价格 先画上半部分

    pen.setColor(Qt::red);
    painter.setPen(pen);
    float Proportional;

    for( int i = 5; i > 0 ; i-- )
    {
         Proportional = (float)fsjl.info.PerValue / 5 / 100 * i;
         str.sprintf( "%.2f%%", Proportional );
         painter.drawText(QPoint(m_windowWidth-BORDER_SIZE-COORDINATE_X2+10,
                                 0+BORDER_SIZE+COORDINATE_Y1+yInterval*(5-i)+2),str);

         str.sprintf("%.2f",(float)fsjl.info.Start / 1000 * (1+Proportional/100));
         painter.drawText(QPoint(0+BORDER_SIZE+COORDINATE_X1-TEXT_LENGTH,
                                 0+BORDER_SIZE+COORDINATE_Y1+yInterval*(5-i)+2),str);
    }

    //画下半部分

    pen.setColor(Qt::green);
    painter.setPen(pen);
    for(int i=1 ; i <= 5 ; i++ )
    {
        str.sprintf( "%.2f%%", Proportional );
        painter.drawText(QPoint(m_windowWidth-BORDER_SIZE-COORDINATE_X2+10,
                                0+BORDER_SIZE+COORDINATE_Y1+yInterval*(5+i)),str);

        str.sprintf("%.2f",(float)fsjl.info.Start / 1000 * (1-Proportional/100));
        painter.drawText(QPoint(0+BORDER_SIZE+COORDINATE_X1-TEXT_LENGTH,
                                0+BORDER_SIZE+COORDINATE_Y1+yInterval*(5+i)+2),str);
    }


    //画时间轴

    pen.setColor(Qt::red);
    painter.setPen(pen);
    for(int i=0 ; i < 9 ; i++)
    {
        double rateI =  (double)( (m_windowWidth-BORDER_SIZE-COORDINATE_X2)- (0+BORDER_SIZE+COORDINATE_X1) ) / 8 + 0.5;
        QPoint ft(BORDER_SIZE+COORDINATE_X2-10+i*(rateI),m_windowHeight-BORDER_SIZE-COORDINATE_Y2+12);
        QDateTime time = QDateTime::fromString("09:30","HH:mm");
        QDateTime newTime;
        if(i >=4){
           newTime = time.addSecs(1800*(i+3));
        }else if(i<4){
           newTime = time.addSecs(1800*i);
        }
        QString str = newTime.toString("HH:mm");
        painter.drawText(ft,str);
    }


    //画分时线
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);

    int zeroX = 0+BORDER_SIZE+COORDINATE_X1;
    int zeroY = 0+BORDER_SIZE+COORDINATE_Y1+yInterval*5;

    float xInter = float(m_windowWidth - 2 * BORDER_SIZE - COORDINATE_X1 -COORDINATE_X2)/241;

    QPoint p1(zeroX,zeroY),p2(zeroX,zeroY);

    for( int i=0; i < 241; i++ )
    {
        if( 240 == i ){
            p2.setX(this->width() - BORDER_SIZE - COORDINATE_X1);
        }else{
            p2.setX(zeroX + i* xInter);
        }
        p2.setY( zeroY - float(fsjl.fsjl[i].Deal - fsjl.info.Start) / fsjl.info.Start / fsjl.info.PerValue * 5* yInterval *10000) ;
        painter.drawLine(p1,p2);
        p1 = p2;
    }

    //画分时线下方的成交量

    float factor = 5 * yInterval / fsjl.info.MaxLevel;

    for( int i=0; i < 241; i++ )
    {
        p2.setX(zeroX + i* xInter);
        p1.setX(zeroX + i* xInter);
        p1.setY(m_windowHeight - BORDER_SIZE - COORDINATE_Y2);
        p2.setY(BORDER_SIZE + COORDINATE_Y2 + 15*yInterval - fsjl.fsjl[i].Vol * factor);
        painter.drawLine(p1,p2);
    }



    //画左侧提示框




















    /*


    //initial 0 and 0%
    QPainter painter(this);
    QPen     pen;
    pen.setColor(Qt::white);
    painter.setPen(pen);
    yInterval  = (double)( (m_windowHeight-BORDER_SIZE-COORDINATE_Y2)- (0+BORDER_SIZE+COORDINATE_Y1) ) / 15 ;
    painter.drawText(QPoint(m_windowWidth-BORDER_SIZE-COORDINATE_X2+10,
                            0+BORDER_SIZE+COORDINATE_Y1+Interval*5 +2),
                     QString("0.00%"));     //右边百分比


    QString str;
    str.sprintf("%.2f",(float)fsjl.info.Start / 1000 );
    // setLSpace( str, 6 - str.size() );
    painter.drawText(QPoint(0+BORDER_SIZE+COORDINATE_X1-TEXT_LENGTH,
                            0+BORDER_SIZE+COORDINATE_Y1+Interval*5 +2),
                     str);     //左边数值



    //actual data

    float Proportional;
    pen.setColor(Qt::red);
    painter.setPen(pen);
    for(int i=5 ; i > 0 ; i-- )
    {
        Proportional = (float)fsjl.info.PerValue / 100 * i;
        str.sprintf( "%.2f%%", Proportional );
        painter.drawText(QPoint(m_windowWidth-BORDER_SIZE-COORDINATE_X2+10,
                                0+BORDER_SIZE+COORDINATE_Y1+Interval*(5-i)+2),
                         str);
        //qDebug()<<"right: "<<Proportional;
        str.sprintf("%.2f",(float)fsjl.info.Start / 1000 * (1+Proportional/100));
        //setLSpace( str, 11 - str.size() );
        painter.drawText(QPoint(0+BORDER_SIZE+COORDINATE_X1-TEXT_LENGTH,
                                0+BORDER_SIZE+COORDINATE_Y1+Interval*(5-i)+2),
                         str);     //左边数值

        //最大值
         if(i == 5){
             m_MaxPrice = str.toFloat();
         }
    }


    pen.setColor(Qt::green);
    painter.setPen(pen);
    for(int i=1 ; i <= 5 ; i++ )
    {
        Proportional = (float)fsjl.info.PerValue / 100 * i;
        str.sprintf( "%.2f%%", Proportional);
        painter.drawText(QPoint(m_windowWidth-BORDER_SIZE-COORDINATE_X2+10,
                                0+BORDER_SIZE+COORDINATE_Y1+Interval*(5+i)),
                         str);

        str.sprintf("%.2f",(float)fsjl.info.Start / 1000 * (1-Proportional/100));
        //setLSpace( str, 11 - str.size() );
        painter.drawText(QPoint(0+BORDER_SIZE+COORDINATE_X1-TEXT_LENGTH,
                                0+BORDER_SIZE+COORDINATE_Y1+Interval*(5+i)+2),
                         str);     //左边数值
        //取最小值
        if( i == 5){
            m_minPrice = str.toFloat();
        }
    }



    //画时刻表
    pen.setColor(Qt::red);
    painter.setPen(pen);

    for(int i=0 ; i < 9 ; i++)
    {
        double rateI =  (double)( (m_windowWidth-BORDER_SIZE-COORDINATE_X2)- (0+BORDER_SIZE+COORDINATE_X1) ) / 8 + 0.5;
        QPoint ft(BORDER_SIZE+COORDINATE_X2-10+i*(rateI),m_windowHeight-BORDER_SIZE-COORDINATE_Y2+12);
        QDateTime time = QDateTime::fromString("09:30","HH:mm");
        QDateTime newTime;
        if(i >=4){
           newTime = time.addSecs(1800*(i+3));
        }else if(i<4){
           newTime = time.addSecs(1800*i);
        }
        QString str = newTime.toString("HH:mm");
        painter.drawText(ft,
                         str);
    }


    //画分时线
    int zeroX = 0+BORDER_SIZE+COORDINATE_X1;
    int zeroY = 0+BORDER_SIZE+COORDINATE_Y1+Interval*5;
    float XInter = float(m_windowWidth - 2 * BORDER_SIZE - COORDINATE_X1 -COORDINATE_X2)/241;
    float YInter = (float)Interval / fsjl.info.PerValue * 100  ;

    int pointX1 = zeroX;
    int pointY1 = zeroY;

    int pointX2 = zeroX;
    int pointY2 = zeroY;

    //set pen
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);

    //draw line

//    for( int i=0; i < 241; i++ )
//    {
//        if(i == 240){
//            pointX2 = this->width()-(90);
//        }else{
//           pointX2 = zeroX + i* XInter;
//        }
//        pointY2 = zeroY - (  float(fsjl.fsjl[i].Deal - fsjl.info.Start)/ fsjl.info.Start )* YInter*100;
//        painter.drawLine(pointX1,pointY1,pointX2,pointY2);
//        pointX1 = pointX2;
//        pointY1 = pointY2;
//    }



    //draw line

    for( int i=0; i < 241; i++ )
    {
        if( 240 == i)
        {
            pointX2 = this->width() - BORDER_SIZE - COORDINATE_X1;
        }
        else
        {
            pointX2 = zeroX + i* XInter;
        }

        pointY2 = zeroY -  float(fsjl.fsjl[i].Deal - fsjl.info.Start) / fsjl.info.Start * YInter * 100;
        painter.drawLine(pointX1,pointY1,pointX2,pointY2);
        pointX1 = pointX2;
        pointY1 = pointY2;

    }



    */
}

void StockCanvas::setLSpace(QString &str, int n)
{
    for( int i=0 ;i<n; i++ )
    {
        str.push_front(" ");
    }
}



void StockCanvas::DrawTips()
{
    //double yGridHight = m_yGridMax - m_yGridMin;  Interval
    //    float YInter = (float)Interval / fsjl.info.PerValue * 100  ;


   // double y_val =  ( 5* yInterval - ( mousePoint.y() - BORDER_SIZE - COORDINATE_Y1 ) )





    //double y_val = ( mousePoint.y() - BORDER_SIZE - COORDINATE_Y1 - 5* yInterval ) * ( price_start * (1+ pervalue/10000)) / (5* yInterval) ;



    //double y_val = price_start / 1000 + ( - mousePoint.y() + BORDER_SIZE + COORDINATE_Y1 + 5* yInterval ) * ( price_start * (1+ pervalue/10000) / 1000 ) /(5* yInterval );


    //double y_val = price_start * (1+ pervalue/10000) / 1000;




   // double y_val = ( mousePoint.y() - BORDER_SIZE - COORDINATE_Y1 - 5* yInterval );






    double y_val = (BORDER_SIZE +  COORDINATE_Y1 + 5* yInterval - mousePoint.y()) * (  (price_start * (1+ pervalue/10000) / 1000  - price_start/1000 ) / (5* yInterval) ) +
                  price_start/1000;


    int iTipsWidth = 60;
    int iTipsHeight = 30;
    QRect rect(0+BORDER_SIZE+COORDINATE_X1-TEXT_LENGTH-15,
               mousePoint.y() - iTipsHeight / 2 ,iTipsWidth,iTipsHeight);
    QPainter painter(this);
    QPen     pen;
    pen.setColor(Qt::white);
    QBrush brush(Qt::blue);
    painter.setBrush(brush);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(rect);
    QString text;
    text.sprintf("%.2f",y_val);
    painter.drawText(rect,Qt::AlignCenter,text);



}





















