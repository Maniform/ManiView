#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE

namespace Ui
{
	class MainWindow;
}

QT_END_NAMESPACE

class MainWindow final : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	void openFile(const QString& fileName);

private:
	Ui::MainWindow* ui;

	void resizeEvent(QResizeEvent* event) override;
	bool event(QEvent* event) override;

	QMargins layoutMargins;
	QMargins graphicsViewMargins;

private slots:
	void on_actionOpen_triggered();

	void fitImage() const;
};
#endif // MAINWINDOW_H
