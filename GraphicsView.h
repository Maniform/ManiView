#ifndef MANIVIEW_GRAPHICSVIEW_H
#define MANIVIEW_GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QImageReader>
#include <QVector2D>

class GraphicsView final : public QGraphicsView
{
	Q_OBJECT

public:
	GraphicsView(QWidget *parent = nullptr);
	~GraphicsView();

	void openFile(const QString& fileName);
	QString getFilePath() const { return filePath; }
	QGraphicsScene& scene() { return current_scene; }
	void fitImage(bool keepTransform = true);
	void nextFrame();
	void clear();

protected:
	void keyPressEvent(QKeyEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	bool event(QEvent* event) override;

private:
	QString filePath;
	QImageReader imageReader;
	QGraphicsPixmapItem* pixmapItem;
	QGraphicsScene current_scene;
	QMap<QString, QString> currentFolderImages;
	bool scaled;
	QVector2D scaleMinMax;

	void loadPreviousImage();
	void loadNextImage();

	uint16_t frameIndex;
	QList<QImage> frames;
};


#endif //MANIVIEW_GRAPHICSVIEW_H