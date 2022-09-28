#ifndef IM_COMMAND_H
#define IM_COMMAND_H

#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QByteArray>
#include "imglobal.h"

namespace ns {
namespace ImCommand {

class GetUploadPhotoMsgResultPrivate;

class BUILDSYS_IM_DECL GetUploadPhotoMsgResult
{
public:
	GetUploadPhotoMsgResult();
	GetUploadPhotoMsgResult(const GetUploadPhotoMsgResult& rhs);
	~GetUploadPhotoMsgResult();

	GetUploadPhotoMsgResult& operator=(const GetUploadPhotoMsgResult& rhs);

	QString receiver() const;
	void setReceiver(const QString& receiver);

	QString sender() const;
	void setSender(const QString& sender);

	QString sendTime() const;
	void setSendTime(const QString& sendTime);

	QString photoId() const;
	void setPhotoId(const QString& id);

	QString expire() const;
	void setExpire(const QString& expire);

	QString smallPhoto() const;
	void setSmallPhoto(const QString& smallPhoto);

	QString largePhoto() const;
	void setLargePhoto(const QString& largePhoto);

private:
	QSharedDataPointer<GetUploadPhotoMsgResultPrivate> d;

};

}
}

Q_DECLARE_METATYPE(ns::ImCommand::GetUploadPhotoMsgResult)

#endif // IM_COMMAND_H