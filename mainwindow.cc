#include "mainwindow.hh"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QStringList download_list_header;
	download_list_header << QString("下载列表") << QString("状态");

	this->ui->download_list->setColumnCount(2);
	this->ui->download_list->setHorizontalHeaderLabels(download_list_header);
	this->ui->download_list->setColumnWidth(0, 600);
	this->ui->download_list->setColumnWidth(1, 70);
	this->ui->download_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
	this->ui->download_list->horizontalHeader()->setStretchLastSection(true);

	this->__downloads = 0;

	return;
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::start_download(QString video_URL, QString audio_URL, QString part, QString bvid, QString save_path)
{
	this->ui->download_list->setRowCount(++this->__downloads);

	this->ui->download_list->setItem(this->__downloads - 1, 0, new QTableWidgetItem(part));
	this->ui->download_list->setItem(this->__downloads - 1, 1, new QTableWidgetItem("下载中..."));
	this->ui->download_list->item(this->__downloads - 1, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	QThread *thread = new QThread();
	Downloader *worker = new Downloader();

	worker->set_download_args(video_URL, audio_URL, part, bvid, this->__downloads - 1, save_path);
	worker->moveToThread(thread);

	connect(thread, &QThread::started, worker, &Downloader::process);
	connect(worker, &Downloader::finished, thread, &QThread::quit);
	connect(worker, &Downloader::finished, this, [this] (int index) {
		this->ui->download_list->setItem(index, 1, new QTableWidgetItem("下载完成"));
		this->ui->download_list->item(index, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		return;
	});
	connect(worker, &Downloader::finished, worker, &Downloader::deleteLater);
	connect(thread, &QThread::finished, thread, &QThread::deleteLater);

	thread->start();

	return;
}

void MainWindow::on_pushButton_clicked()
{
	URLList *urllist = new URLList();

	QObject::connect(urllist, SIGNAL(ready_download(QString,QString,QString,QString,QString)), this, SLOT(start_download(QString,QString,QString,QString,QString)));
	urllist->parsing(this->ui->url_edit->text());

	return;
}
