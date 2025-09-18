#ifndef VIEWER_VIEWERAPPLICATION_H
#define VIEWER_VIEWERAPPLICATION_H

#include <QApplication>

class MainWindow;

class ViewerApplication final : public QApplication
{
	Q_OBJECT

public:
	ViewerApplication(int& argc, char** argv);

	void setMainWindow(MainWindow* inW);

protected:
	bool event(QEvent* event) override;

private:
	MainWindow* w;
};


#endif //VIEWER_VIEWERAPPLICATION_H
