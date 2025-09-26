#include "GraphicsView.h"

#include <QDirIterator>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QMessageBox>
#include <QScrollBar>
#include <QTimer>

GraphicsView::GraphicsView(QWidget* parent)
	: QGraphicsView(parent)
	, pixmapItem(nullptr)
	, frameIndex(0)
	, scaled(false)
	, scaleMinMax(0.1f, 3.f)
{
	setScene(&current_scene);
	scene().setBackgroundBrush(QBrush(Qt::darkGray));
}

GraphicsView::~GraphicsView()
{
}

void GraphicsView::openFile(const QString& fileName)
{
	frames.clear();
	frameIndex = 0;

	const QFileInfo fileInfo(fileName);
	const QList<QByteArray> supportedImageFormats = QImageReader::supportedImageFormats();
	if (!supportedImageFormats.contains(fileInfo.suffix()))
	{
		QMessageBox::critical(this, tr("Open file"), tr("Incompatible file format : ") + fileInfo.suffix());
		return;
	}

	imageReader.setFileName(fileName);
	if (imageReader.canRead())
	{
		const QPixmap pixmap = QPixmap::fromImageReader(&imageReader);
		if (!pixmap.isNull())
		{
			filePath = fileName;
			if (!pixmapItem)
			{
				pixmapItem = current_scene.addPixmap(pixmap);
				pixmapItem->setTransformOriginPoint(pixmapItem->boundingRect().center());
			}
			else
			{
				pixmapItem->setPixmap(pixmap);
			}
			fitImage(false);

			if (imageReader.supportsAnimation())
			{
				QTimer::singleShot(imageReader.nextImageDelay(), this, &GraphicsView::nextFrame);
			}
		}

		QDirIterator dirIterator(fileInfo.path(), QDir::Files);
		while (dirIterator.hasNext())
		{
			const QFileInfo nextFileInfo(dirIterator.next());
			if (supportedImageFormats.contains(nextFileInfo.suffix()))
			{
				currentFolderImagesAndPath[nextFileInfo.fileName()] = nextFileInfo.filePath();
			}
		}
	}
}

void GraphicsView::fitImage(bool keepTransform)
{
	if (pixmapItem)
	{
		if (!keepTransform)
		{
			scaled = false;
		}
		const QTransform previousTransform = transform();
		const QVector2D scrollBars(
			static_cast<float>(horizontalScrollBar()->value()) / horizontalScrollBar()->maximum(),
			static_cast<float>(verticalScrollBar()->value()) / verticalScrollBar()->maximum()
		);
		current_scene.setSceneRect(pixmapItem->boundingRect());
		fitInView(pixmapItem, Qt::KeepAspectRatio);
		if (keepTransform && scaled)
		{
			setTransform(previousTransform);
			horizontalScrollBar()->setValue(scrollBars.x() * horizontalScrollBar()->maximum());
			verticalScrollBar()->setValue(scrollBars.y() * verticalScrollBar()->maximum());
		}
	}
}

void GraphicsView::rotateRight()
{
	if (pixmapItem)
	{
		float rotation = pixmapItem->rotation() + 90.f;
		rotation = std::fmod(rotation, 360.f);
		pixmapItem->setRotation(rotation);
		fitImage();
	}
}

void GraphicsView::nextFrame()
{
	if (pixmapItem && imageReader.supportsAnimation())
	{
		const QPixmap pixmap = QPixmap::fromImageReader(&imageReader);
		pixmapItem->setPixmap(pixmap);
		if (imageReader.currentImageNumber() >= imageReader.imageCount() - 1)
		{
			imageReader.setFileName(filePath);
		}
		QTimer::singleShot(imageReader.nextImageDelay(), this, &GraphicsView::nextFrame);
	}
}

void GraphicsView::clear()
{
	if (pixmapItem)
	{
		current_scene.clear();
		pixmapItem = nullptr;
	}
}

void GraphicsView::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Left:
		loadPreviousImage();
		break;

	case Qt::Key_Right:
		loadNextImage();
		break;

	default:
		break;
	}

	QGraphicsView::keyPressEvent(event);
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent* event)
{
	fitImage(false);
	QGraphicsView::mouseDoubleClickEvent(event);
}

bool GraphicsView::event(QEvent* event)
{
	switch (event->type())
	{
	case QEvent::NativeGesture:
		{
			QNativeGestureEvent* nativeGesture = static_cast<QNativeGestureEvent*>(event);
			const float zoomValue = nativeGesture->value();
			const float zoom = 1.f + zoomValue;
			scale(zoom, zoom);
			scaled = true;
			break;
		}

	default:
		break;
	}
	return QGraphicsView::event(event);
}

void GraphicsView::loadPreviousImage()
{
	if (pixmapItem)
	{
		const QFileInfo fileInfo(filePath);
		QMap<QString, QString>::iterator fileIterator = currentFolderImagesAndPath.find(fileInfo.fileName());
		if (fileIterator == currentFolderImagesAndPath.begin())
		{
			fileIterator = std::prev(currentFolderImagesAndPath.end());
		}
		else
		{
			--fileIterator;
		}

		if (filePath != fileIterator.value())
		{
			openFile(fileIterator.value());
		}
	}
}

void GraphicsView::loadNextImage()
{
	if (pixmapItem)
	{
		const QFileInfo fileInfo(filePath);
		QMap<QString, QString>::iterator fileIterator = currentFolderImagesAndPath.find(fileInfo.fileName());
		if (fileIterator == currentFolderImagesAndPath.end())
		{
			fileIterator = currentFolderImagesAndPath.begin();
		}
		else
		{
			++fileIterator;
			if (fileIterator == currentFolderImagesAndPath.end())
			{
				fileIterator = currentFolderImagesAndPath.begin();
			}
		}

		if (filePath != fileIterator.value())
		{
			openFile(fileIterator.value());
		}
	}
}
