#include "downloader.h"

Downloader::Downloader(QObject *parent) : QObject(parent)
{
	return;
}

void Downloader::process()
{
	video_manager = new QNetworkAccessManager();
	audio_manager = new QNetworkAccessManager();
	video_request = new QNetworkRequest();
	audio_request = new QNetworkRequest();

	video_request->setUrl(QUrl(this->__download_video_URLs));
	video_request->setRawHeader("user-agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.121 Safari/537.36 ");
	video_request->setRawHeader("referer", "https://www.bilibili.com");
	audio_request->setUrl(QUrl(this->__download_audio_URLS));
	audio_request->setRawHeader("user-agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.121 Safari/537.36 ");
	audio_request->setRawHeader("referer", "https://www.bilibili.com");

	video_file = new QFile(this->__base_save_path + this->__download_parts + "v.m4s");
	audio_file = new QFile(this->__base_save_path + this->__download_parts + "a.m4s");
	video_file->open(QIODevice::WriteOnly);
	audio_file->open(QIODevice::WriteOnly);

	audio_reply = audio_manager->get(*audio_request);

	connect(audio_reply, &QNetworkReply::readyRead, this, [this] () {
		if (this->audio_file)
			this->audio_file->write(this->audio_reply->readAll());
	});

	connect(audio_reply, &QNetworkReply::finished, this, [this] () {
		this->audio_file->flush();
		this->audio_file->close();
		this->audio_reply->deleteLater();

		this->video_reply = this->video_manager->get(*this->video_request);

		connect(video_reply, &QNetworkReply::readyRead, this, [this] () {
			if (this->video_file)
				this->video_file->write(this->video_reply->readAll());
		});

		connect(video_reply, &QNetworkReply::finished, this, [this] () {
			this->video_file->flush();
			this->video_file->close();
			this->video_reply->deleteLater();

			QString ffmpeg_exec = "ffmpeg";
			QStringList ffmpeg_args;
			QProcess *ffmpeg_proc = new QProcess();

			ffmpeg_args << "-i" << video_file->fileName() << "-i" << audio_file->fileName() << "-codec" << "copy" << this->__base_save_path + this->__download_parts + ".mp4" << "-y";
			ffmpeg_proc->start(ffmpeg_exec, ffmpeg_args);
			ffmpeg_proc->waitForFinished();

			video_file->remove();
			audio_file->remove();

			emit finished(__index);
		});
	});

	return;
}

void Downloader::set_download_args(QString video_URL, QString audio_URL, QString part, QString bvid, int index, QString save_path)
{
	this->__index = index;
	this->__download_video_URLs = video_URL;
	this->__download_audio_URLS = audio_URL;
	this->__download_parts = part;
	this->__bvids = bvid;
	this->__base_save_path = save_path;
	return;
}
