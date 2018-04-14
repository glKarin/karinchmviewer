#ifndef _KARIN_SERVICE_H
#define _KARIN_SERVICE_H

#include <MApplicationService>

namespace karin
{
	class karin_service : public MApplicationService
	{
		public:
			karin_service(const QString &serviceName, QObject *parent = 0);
			virtual ~karin_service();
			virtual void launch();
			virtual void handleServiceRegistrationFailure();
	};
}

#endif
