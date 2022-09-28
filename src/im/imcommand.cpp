#include "ImCommand.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "ImCommand:"
#endif // DEBUG_PREFIX

namespace ns {
namespace ImCommand {

class GetUploadPhotoMsgResultPrivate : public QSharedData 
{
public:
	GetUploadPhotoMsgResultPrivate();
	GetUploadPhotoMsgResultPrivate(const GetUploadPhotoMsgResultPrivate& rhs);

	QString m_receiver;
	QString m_sender;
	QString m_sendTime;
	QString m_photoId;
	QString m_expire;
	QString m_smallPhoto;
	QString m_largePhoto;
};

GetUploadPhotoMsgResultPrivate::GetUploadPhotoMsgResultPrivate() :
	QSharedData(),
	m_receiver(),
	m_sender(),
	m_sendTime(),
	m_photoId(),
	m_expire(),
	m_smallPhoto(),
	m_largePhoto()
{

}

GetUploadPhotoMsgResultPrivate::GetUploadPhotoMsgResultPrivate(const GetUploadPhotoMsgResultPrivate& rhs)
	: QSharedData(rhs),
	m_receiver(rhs.m_receiver),
	m_sender(rhs.m_sender),
	m_sendTime(rhs.m_sendTime),
	m_photoId(rhs.m_photoId),
	m_expire(rhs.m_expire),
	m_smallPhoto(rhs.m_smallPhoto),
	m_largePhoto(rhs.m_largePhoto)
{
	
}

GetUploadPhotoMsgResult::GetUploadPhotoMsgResult() :
	d(new GetUploadPhotoMsgResultPrivate)
{

}

GetUploadPhotoMsgResult::GetUploadPhotoMsgResult(const GetUploadPhotoMsgResult& rhs) :
	d(rhs.d)
{

}

GetUploadPhotoMsgResult::~GetUploadPhotoMsgResult()
{

}

GetUploadPhotoMsgResult& GetUploadPhotoMsgResult::operator=(const GetUploadPhotoMsgResult& rhs)
{
	d = rhs.d;
	return *this;
}

QString GetUploadPhotoMsgResult::receiver() const 
{
	return d->m_receiver;
}

void GetUploadPhotoMsgResult::setReceiver(const QString& receiver)
{
	d->m_receiver = receiver;
}

QString GetUploadPhotoMsgResult::sender() const
{
	return d->m_sender;
}

void GetUploadPhotoMsgResult::setSender(const QString& sender)
{
	d->m_sender = sender;
}

QString GetUploadPhotoMsgResult::sendTime() const 
{
	return d->m_sendTime;
}

void GetUploadPhotoMsgResult::setSendTime(const QString& sendTime)
{
	d->m_sendTime = sendTime;
}


QString GetUploadPhotoMsgResult::photoId() const 
{
	return d->m_photoId;
}

void GetUploadPhotoMsgResult::setPhotoId(const QString& id)
{
	d->m_photoId = id;
}

QString GetUploadPhotoMsgResult::expire() const 
{
	return d->m_expire;
}

void GetUploadPhotoMsgResult::setExpire(const QString& expire)
{
	d->m_expire = expire;
}

QString GetUploadPhotoMsgResult::smallPhoto() const 
{
	return d->m_smallPhoto;
}

void GetUploadPhotoMsgResult::setSmallPhoto(const QString& smallPhoto)
{
	d->m_smallPhoto = smallPhoto;
}

QString GetUploadPhotoMsgResult::largePhoto() const 
{
	return d->m_largePhoto;
}

void GetUploadPhotoMsgResult::setLargePhoto(const QString& largePhoto)
{
	d->m_largePhoto = largePhoto;
}


} // namespace ns::ImCommand
} // namespace ns
