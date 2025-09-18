#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>

#include "GraphicsView.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openFile(const QString& fileName)
{
	ui->graphicsView->openFile(fileName);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	ui->graphicsView->fitImage();
	QMainWindow::resizeEvent(event);
}

void MainWindow::on_actionOpen_triggered()
{
	const QList<QByteArray> supportedImageFormats = QImageReader::supportedImageFormats();
	QString filter;
	for (const QByteArray& imageFormat : supportedImageFormats)
	{
		if (imageFormat == supportedImageFormats[0])
		{
			filter = "*." + imageFormat;
		}
		else
		{
			filter += QString(";") + "*." + imageFormat;
		}
	}

	const QString fileName = QFileDialog::getOpenFileName(this, tr("Ouvrir une image"), ui->graphicsView->getFilePath(), filter);
	if (!fileName.isEmpty())
	{
		ui->graphicsView->clear();
		ui->graphicsView->openFile(fileName);
	}
}
