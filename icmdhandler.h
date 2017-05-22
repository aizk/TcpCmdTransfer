#ifndef ICMDHANDLER_H
#define ICMDHANDLER_H

#include <QtCore>

class ICmdHandler : public QObject
{
public:
	virtual ~ICmdHandler();
    virtual int handleCmd(QByteArray &aCmd) = 0;

};

typedef ICmdHandler *PCmdHandler;
typedef QList<PCmdHandler> ListPCmdHandler;

#endif // ICMDHANDLER_H
