#ifndef DOWNLOADER_HH
#define DOWNLOADER_HH

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QProcess>

class Downloader : public QObject
{
	Q_OBJECT
public:
	explicit Downloader(QObject *parent = nullptr);

	void set_download_args(QString, QString, QString, QString, int, QString);

signals:
	void finished(int);

public slots:
	void process();

private:
	int __index;
	QString __download_video_URLs;
	QString __download_audio_URLS;
	QString __download_parts;
	QString __bvids;
	QFile *video_file;
	QFile *audio_file;
	QNetworkReply *video_reply;
	QNetworkReply *audio_reply;

	QNetworkAccessManager *video_manager;
	QNetworkAccessManager *audio_manager;
	QNetworkRequest *video_request;
	QNetworkRequest *audio_request;

	QString __base_save_path;
};

#endif // DOWNLOADER_HH
