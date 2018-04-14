#include "karin_service.h"

#include <QDebug>

karin::karin_service::karin_service(const QString &serviceName, QObject *parent)
	:MApplicationService(QString("com.nokia.") + serviceName, parent)
{
}

karin::karin_service::~karin_service()
{
}

void karin::karin_service::launch()
{
	MApplicationService::launchAnotherWithQProcess();
}

void karin::karin_service::handleServiceRegistrationFailure()
{
	qDebug() << "MyApplicationService::handleServiceRegistrationFailure()";

	MApplicationService::incrementAndRegister();
}
