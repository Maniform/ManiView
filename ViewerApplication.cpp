#include "ViewerApplication.h"

#include <QFileOpenEvent>

#include "MainWindow.h"

ViewerApplication::ViewerApplication(int& argc, char** argv)
	: QApplication(argc, argv)
	  , w(nullptr)
{
}

void ViewerApplication::setMainWindow(MainWindow* inW)
{
	w = inW;
}

bool ViewerApplication::event(QEvent* event)
{
	switch (event->type())
	{
	case QEvent::FileOpen:
		if (const QFileOpenEvent* fileOpenEvent = dynamic_cast<QFileOpenEvent*>(event))
		{
			if (w)
			{
				w->openFile(fileOpenEvent->file());
			}
		}
		event->setAccepted(true);
		break;

	default:
		break;
	}

	return QApplication::event(event);
}
