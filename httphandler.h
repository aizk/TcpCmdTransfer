#ifndef HTTPHANDLER
#define HTTPHANDLER

#include <QtNetwork>
#include <QNetworkReply>
#include "icmdhandler.h"
#include <curl/curl.h>

class HTTPHandler : public ICmdHandler
{
public:
    HTTPHandler();
	~HTTPHandler();
    int handleCmd(QByteArray &aCmd);
	bool isIdle() const;
	static void readSettings(QSettings &settings);
	static HTTPHandler * getHttpHandler();

protected:
	volatile bool m_isIdle;
	curl_slist *m_headers;
	static QUrl m_serviceUrl;
	static QList<HTTPHandler *> lstHttpHandler;
};

bool isHttpHandlerIdle(const HTTPHandler *pHttpHandler);

#endif // HTTPHANDLER
