#ifndef URLLIST_HH
#define URLLIST_HH

#include <QMainWindow>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QVector>
#include <QUrlQuery>
#include <QUrl>
#include <QStandardPaths>
#include <QFileDialog>

namespace Ui {
class URLList;
}

class URLList : public QMainWindow
{
	Q_OBJECT

public:
	explicit URLList(QWidget *parent = nullptr);
	~URLList();

	void parsing(QString);

signals:
	void ready_download(QString, QString, QString, QString, QString);

private slots:
	void on_download_btn_clicked();

	void on_set_download_path_btn_clicked();

private:
	Ui::URLList *ui;

	QString __bvid;
	QVector<int> __cids;
	QVector<QString> __parts;
	QVector<QString> __video_download_URL;
	QVector<QString> __audio_download_URL;

	QString __download_path;
};

#endif // URLLIST_HH
