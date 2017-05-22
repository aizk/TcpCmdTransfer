#include "httphandler.h"
#include "const.h"
#include <QtCore>
#include <iostream>
#include <algorithm>
#include "utils.h"

QUrl HTTPHandler::m_serviceUrl;
QList<HTTPHandler *> HTTPHandler::lstHttpHandler;
static char CONTENT_TYPE[] = "Content-Type: Application/json";
static char KEY_SERVICE_URL[] = "ServiceUrl";

HTTPHandler::HTTPHandler()
	: m_headers(NULL),
	  m_isIdle(true)
{
	m_headers = curl_slist_append(m_headers, CONTENT_TYPE);
	CURLcode rtnCode = curl_global_init(CURL_GLOBAL_WIN32);
}

int HTTPHandler::handleCmd(QByteArray &aCmd)
{
	//m_isIdle = false;
	//Utils::logCmd(Const::HTTP_HANDLER, aCmd, Utils::JSON_CMD);
	//CURL *curl = curl_easy_init();
	//if (curl)
	//{
	//	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
	//	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1); 
	//	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
	//	curl_easy_setopt(curl, CURLOPT_POST, 1);  
	//	curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1);
	//	curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 5);
	//	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
	//	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, m_headers);
	//	curl_easy_setopt(curl, CURLOPT_URL, m_serviceUrl.toString().toUtf8().data());
	//	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, aCmd.data()); 
	//	CURLcode rtnCode = curl_easy_perform(curl);   // Ö´ÐÐ
	//	if(CURLE_OK == rtnCode)
	//	{
	//		Utils::logCmd(Const::HTTPPOST_OK, aCmd, Utils::JSON_CMD);
	//	}
	//	curl_easy_cleanup(curl);
	//	m_isIdle = true;
	//	return 0;
	//}
	//m_isIdle = true;
	//return Const::RTN_ERROR;
	return 0;
}

HTTPHandler::~HTTPHandler()
{
}

void HTTPHandler::readSettings( QSettings &settings )
{
	m_serviceUrl = settings.value(KEY_SERVICE_URL).toUrl();
	Q_ASSERT(m_serviceUrl.isValid());
}

HTTPHandler * HTTPHandler::getHttpHandler()
{
	if(lstHttpHandler.size() < QThreadPool::globalInstance()->maxThreadCount())
	{
		HTTPHandler * pNewHttpHandler = new HTTPHandler();
		lstHttpHandler.append(pNewHttpHandler);
		return pNewHttpHandler;
	}
	QList<HTTPHandler *>::iterator it = std::find_if(lstHttpHandler.begin(), lstHttpHandler.end(), isHttpHandlerIdle);
	if(it != lstHttpHandler.end())
	{
		return *it;
	}
	else
	{
		return lstHttpHandler.at(0);
	}
}

bool HTTPHandler::isIdle() const
{
	return m_isIdle;
}


bool isHttpHandlerIdle(const HTTPHandler *pHttpHandler)
{
	return pHttpHandler->isIdle();
}
