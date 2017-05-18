#include "qmydata.h"
QMyData::QMyData()
{    
    memset(fsjl,0,sizeof(FSJL)*241);
}

QMyData::QMyData( char* SecID, char* Date )
{
    strcpy(szSecID , SecID );
    strcpy( szDate , Date) ;
    memset(fsjl,0,sizeof(FSJL)*241);
}

QMyData::~QMyData()
{
    if( nullptr != file )
    file->close();
}


void QMyData::GetFSJLINFO()
{
    double UpRate = 0, DnRate = 0;
    info.deal_Start = info.deal_Max = info.deal_Min = fsjl[0].Deal;
    info.vol_Max = info.vol_Min = 0;

    for( int i=1; i<241; i++ )
    {
        if( info.deal_Max < fsjl[i].Deal )
            info.deal_Max = fsjl[i].Deal;
        if( info.deal_Min > fsjl[i].Deal )
            info.deal_Min = fsjl[i].Deal;

        if(info.vol_Max < fsjl[i].Vol)
            info.vol_Max = fsjl[i].Vol;
        if(info.vol_Min > fsjl[i].Vol)
            info.vol_Min = fsjl[i].Vol;
    }    

    qDebug("%d",info.deal_Max);

    if( info.deal_Max > info.deal_Start)
    {
        UpRate = ( info.deal_Max - info.deal_Start ) / info.deal_Start ;
    }
    if( info.deal_Min < info.deal_Start)
    {
        DnRate = ( info.deal_Start - info.deal_Min ) / info.deal_Start ;
    }

    info.deal_rate = UpRate > DnRate ? UpRate : DnRate ;

}



bool QMyData::ReadFSJL()
{
    QString str("D:\\FSJL.000001K.20100310");
    char    line[1024];
    char    *token;
    //str.sprintf(+"FSJL.%s.%s",szSecID,szDate);

    file = new QFile(str);
    if( !file->open(QFile::ReadOnly) )
        return false;

    for( int i=0; i < 241; i++ )
    {
        if( file->readLine(line,1024)  > 0 )
        {
            token = strtok( line, "|" );
            if( token != NULL )
                fsjl[i].Date = atoi(token);

            token = strtok( NULL, "|" );
            if( token != NULL )
                fsjl[i].Time = atoi(token);

            token = strtok( NULL, "|" );
            if( token != NULL )
                strcpy(fsjl[i].SecID,token);

            token = strtok( NULL, "|" );
            if( token != NULL )
                strcpy(fsjl[i].SecName,token);

            token = strtok( NULL, "|" );
            if( token != NULL )
                fsjl[i].Deal = atoi(token);

            token = strtok( NULL, "|" );
            if( token != NULL )
                fsjl[i].Vol = atoi(token);

            token = strtok( NULL, "|" );
            if( token != NULL )
                fsjl[i].Amt = atoi(token);
        }
    }
    return true;
}

