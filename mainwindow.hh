#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QFile>
#include <QProcess>
#include <QThread>

#include "urllist.hh"
#include "downloader.hh"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_pushButton_clicked();

	void start_download(QString, QString, QString, QString, QString);

private:
	Ui::MainWindow *ui;

	int __downloads;
};
#endif // MAINWINDOW_HH
