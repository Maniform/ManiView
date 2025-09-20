#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QTimer>
#include <QWindowStateChangeEvent>

#include "GraphicsView.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	if (const QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(ui->centralwidget->layout()))
	{
		layoutMargins = layout->contentsMargins();
	}
	graphicsViewMargins = ui->graphicsView->contentsMargins();
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

bool MainWindow::event(QEvent* event)
{
	switch (event->type())
	{
	case QEvent::WindowStateChange:
		{
			if (QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(ui->centralwidget->layout()))
			{
				if (windowState() == Qt::WindowFullScreen)
				{
					layout->setContentsMargins(QMargins());
					ui->graphicsView->setContentsMargins(QMargins());
					ui->statusbar->hide();
				}
				else
				{
					layout->setContentsMargins(layoutMargins);
					ui->graphicsView->setContentsMargins(graphicsViewMargins);
					ui->statusbar->show();
				}
				QTimer::singleShot(0, this, &MainWindow::fitImage);
			}
			break;
		}

	case QEvent::KeyPress:
		if (const QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event))
		{
			switch (keyEvent->key())
			{
			case Qt::Key_Escape:
				if (windowState() == Qt::WindowFullScreen)
				{
					setWindowState(Qt::WindowNoState);
				}
				break;

			default:
				break;
			}
		}
		break;

	default:
		break;
	}
	return QMainWindow::event(event);
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

void MainWindow::fitImage() const
{
	ui->graphicsView->fitImage();
}
