/* This file is in public domain. */

//#include <BreakpadHttpSender.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <SmtpMime>

#ifndef DEBUG_PREFIX
#define DEBUG_PREFIX "CrashReport: "
#endif // DEBUG_PREFIX

int main(int argc, char* argv[])
{
	QCoreApplication app(argc, argv);
 	if(argc < 2 || argc > 3) {
  		qWarning() << DEBUG_PREFIX << "Usage: CrashReport <dump file> ";
  		return -1;
 	}

	qWarning() << DEBUG_PREFIX << "param correct";

	SmtpClient smtp(QLatin1String("smtp.163.com"), 465, SmtpClient::SslConnection);

	smtp.setCrash(true);

	smtp.setUser(QLatin1String("alks651843787@163.com"));
	smtp.setPassword(QLatin1String("nba8999761"));

	MimeMessage message;

	EmailAddress sender(QLatin1String("alks651843787@163.com"), QLatin1String("Your Name"));
	message.setSender(&sender);

	EmailAddress to(QLatin1String("haris.chan@yespo.com"), QLatin1String("Recipient's Name"));
	message.addRecipient(&to);

// 	EmailAddress rcpt(QLatin1String("alks651843787@gmail.com"), QLatin1String("Recipient's Name"));
// 	message.addRecipient(&rcpt);

	message.setSubject(QLatin1String("CrashReport for YesPoPartner"));

	MimeText text;
	text.setText(QLatin1String("This is an email with crash report files."));
	message.addPart(&text);

	QFile *file = new QFile(app.arguments().at(1));

	qWarning() << DEBUG_PREFIX << "crashReport" << file->fileName();

	MimeAttachment attachment (file);

	message.addPart(&attachment);

	if (!smtp.connected()) {
		qWarning() << DEBUG_PREFIX << "SMTP Failed to connect to host!" << endl;
		return -2;
	}

	if (!smtp.login()) {
		qWarning() << DEBUG_PREFIX << "SMTP Failed to login!" << endl;
		return -3;
	}

	if (!smtp.sendMail(message)) {
		qWarning() << DEBUG_PREFIX << "SMTP Failed to send mail!" << endl;
		return -4;
	}

	file->close();

	file->deleteLater();
	return 0;

}
