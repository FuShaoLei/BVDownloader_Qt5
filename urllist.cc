#include "urllist.hh"
#include "ui_urllist.h"

URLList::URLList(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::URLList)
{
	ui->setupUi(this);

	this->__download_path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/";
	this->ui->download_path->setText(this->__download_path);

	return;
}

URLList::~URLList()
{
	delete ui;
}

void URLList::parsing(QString bvid)
{
	this->__bvid = bvid;
	this->ui->bvid_label->setText(bvid);

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	QNetworkRequest request;

	request.setUrl("https://api.bilibili.com/x/player/pagelist?bvid=" + bvid);

	connect(manager, &QNetworkAccessManager::finished, this, [this] (QNetworkReply *reply) {
		QString result_string = reply->readAll();
		reply->deleteLater();

		QJsonArray data = QJsonDocument::fromJson(result_string.toUtf8().data()).object().value("data").toArray();

		for (int ___ = 0; ___ < data.size(); ++___) {
			__cids.push_back(data.at(___).toObject().value("cid").toInt());
			__parts.push_back(data.at(___).toObject().value("part").toString());
		}

		QStringList video_list_header;
		video_list_header << QString("视频名称") << QString("时长");

		this->ui->video_list->setColumnCount(2);
		this->ui->video_list->setRowCount(data.size());
		this->ui->video_list->setHorizontalHeaderLabels(video_list_header);
		this->ui->video_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
		this->ui->video_list->setColumnWidth(0, 400);
		this->ui->video_list->setColumnWidth(1, 70);
		this->ui->video_list->horizontalHeader()->setStretchLastSection(true);

		for (int ___ = 0; ___ < data.size(); ++___)
			this->ui->video_list->setItem(___, 0, new QTableWidgetItem(data.at(___).toObject().value("part").toString()));

		QNetworkAccessManager *download_manager = new QNetworkAccessManager(this);
		QNetworkRequest download_request;

		connect(download_manager, &QNetworkAccessManager::finished, this, [this] (QNetworkReply *reply) {
			int id = (new QUrlQuery(reply->url()))->queryItemValue("id").toInt();
			QString result_string = reply->readAll();
			reply->deleteLater();

			QJsonDocument result_json = QJsonDocument::fromJson(result_string.toUtf8().data());
			QJsonObject data = result_json.object().value("data").toObject();
			QJsonObject dash = data.value("dash").toObject();
			QJsonArray video = dash.value("video").toArray();
			QJsonArray audio = dash.value("audio").toArray();
			QJsonObject video_0 = video.at(1).toObject();
			QJsonObject audio_0 = audio.at(0).toObject();

			QString video_URL = video_0.value("baseUrl").toString();
			QString audio_URL = audio_0.value("baseUrl").toString();

			this->__video_download_URL[id] = video_URL;
			this->__audio_download_URL[id] = audio_URL;

			int time_length = data.value("timelength").toInt();
			int ss = 1000;
			int mi = ss * 60;
			int hh = mi * 60;
			int dd = hh * 24;
			long day = time_length / dd;
			long hour = (time_length - day * dd) / hh;
			long minute = (time_length - day * dd - hour * hh) / mi;
			long second = (time_length - day * dd - hour * hh - minute * mi) / ss;

			QString hou = QString("%1").arg(hour, 2, 10, QChar('0'));
			QString min = QString("%1").arg(minute, 2, 10, QChar('0'));
			QString sec = QString("%1").arg(second, 2, 10, QChar('0'));
			QString time = hou + ":" + min + ":" + sec;

			this->ui->video_list->setItem(id, 1, new QTableWidgetItem(time));
			this->ui->video_list->item(id, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

			return;
		});

		this->__audio_download_URL.resize(this->__cids.size());
		this->__video_download_URL.resize(this->__cids.size());

		for (int ___ = 0; ___ < __cids.size(); ++___) {
			QUrl request_url("https://api.bilibili.com/x/player/playurl");
			QUrlQuery request_url_query(request_url);

			request_url_query.addQueryItem("bvid", this->__bvid);
			request_url_query.addQueryItem("cid", QString::number(__cids[___]));
			request_url_query.addQueryItem("fnval", QString::number(16));
			request_url_query.addQueryItem("qn", QString::number(80));
			request_url_query.addQueryItem("id", QString::number(___));
			request_url.setQuery(request_url_query);

			download_request.setUrl(request_url);
			download_request.setRawHeader("cookie", "SESSDATA=");

			download_manager->get(download_request);
		}

		this->show();
		return;
	});

	manager->get(request);

	return;
}

void URLList::on_download_btn_clicked()
{
	QList<QTableWidgetItem *> select_items = this->ui->video_list->selectedItems();
	int items = select_items.size();

	if (select_items.size() == 0)
		return;

	for (int ___ = 0; ___ < items; ++___) {
		int index = this->ui->video_list->row(select_items.at(___));

		emit ready_download(this->__video_download_URL[index],
								  this->__audio_download_URL[index],
								  this->__parts[index],
								  this->__bvid,
								  this->__download_path);

		this->close();
	}

	return;
}

void URLList::on_set_download_path_btn_clicked()
{
	this->__download_path = QFileDialog::getExistingDirectory(NULL, "选择保存路径", this->__download_path) + "/";
	this->ui->download_path->setText(this->__download_path);

	return;
}
