#pragma once

#include <QString>


enum MethodType
{
	eGetMethod = 0,
	ePostMethod,
	ePatchMethod,
	eDeleteMethod,
};


class NetEntity
{
public:
                        NetEntity(const MethodType method = ePostMethod);
						~NetEntity();

		MethodType		getMethodType() const { return methodType; }
        void			setMethodType(const MethodType method) { methodType = method; }

		QString			getMethodUrl() const { return methodUrl; }
        void			setMethodUrl(const QString& url) { methodUrl = url; }

		QString			getRequestBody() const { return requestBody; }
        void			setRequestBody(const QString& jsonRequest) { requestBody = jsonRequest; }

		QString			getResult() const { return result; }
        void			setResult(const QString& jsonResult) { result = jsonResult; }

private:
	MethodType		methodType;			// method request type
	QString			methodUrl;			// e.g. "meal/get"
	QString			requestBody;		// Json
	QString			result;				// Json
};
