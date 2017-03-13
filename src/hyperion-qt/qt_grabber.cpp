
// Hyperion-AmLogic includes
#include "qt_grabber.h"
#include <QPixmap>
#include <QWindow>
#include <QGuiApplication>
#include <QWidget>
#include <QColor>

QtGrabber::QtGrabber(const unsigned display, const unsigned grabWidth, const unsigned grabHeight, const unsigned updateRate_Hz)
	: _timer(this)
	, _width(grabWidth)
	, _height(grabHeight)
	, _screenshot(grabWidth, grabHeight)
	, _colorDlg(this)
{
	_timer.setSingleShot(false);
	_timer.setInterval(updateRate_Hz);
	createTrayIcon();

	// Connect capturing to the timeout signal of the timer
	connect(&_timer, SIGNAL(timeout()), this, SLOT(capture()));
	connect(_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

	connect(&_colorDlg, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(newColor(const QColor &)));
	QIcon icon = QIcon::fromTheme("video-display");
	_trayIcon->setIcon(icon);
	_trayIcon->show();
	setWindowIcon(icon);
	//_colorDlg.setModal(false);
	//_colorDlg.show();
	_colorDlg.setOptions(QColorDialog::NoButtons);
	json = new JsonConnection("localhost:19444");
}

QtGrabber::~QtGrabber()
{
	delete json;
}

void QtGrabber::iconActivated(QSystemTrayIcon::ActivationReason reason)
{

	switch (reason)
	{
		case QSystemTrayIcon::Trigger:
			setXColor();
			break;
		case QSystemTrayIcon::DoubleClick:
			break;
		case QSystemTrayIcon::MiddleClick:
			//showMessage();
			break;
		default: ;
	}
 }

 
void QtGrabber::setXColor()
{
	if(_colorDlg.isVisible())
	{
	_colorDlg.hide();
	}
	else
	{
	_colorDlg.show();
	newColor(_colorDlg.currentColor());
	}
}

void QtGrabber::newColor(const QColor & color)
{
	stop();
	std::vector<QColor> colorVector;
	colorVector.push_back(color);

	json->setColor(colorVector, 150, 0);
}

void QtGrabber::createTrayIcon()
{
	quitAction = new QAction(tr("&Quit"), this);
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

	startAction = new QAction(tr("St&art capture"), this);
	connect(startAction, SIGNAL(triggered()), this, SLOT(start()));

	stopAction = new QAction(tr("St&op capture"), this);
	connect(stopAction, SIGNAL(triggered()), this, SLOT(stop()));

	colorAction = new QAction(tr("&Color"), this);
	connect(colorAction, SIGNAL(triggered()), this, SLOT(setXColor()));

	_trayIconMenu = new QMenu(this);
	_trayIconMenu->addAction(stopAction);
	_trayIconMenu->addAction(startAction);
	_trayIconMenu->addAction(colorAction);
	_trayIconMenu->addSeparator();
	_trayIconMenu->addAction(quitAction);

	_trayIcon = new QSystemTrayIcon(this);
	_trayIcon->setContextMenu(_trayIconMenu);
}

const Image<ColorRgb> & QtGrabber::getScreenshot()
{
	capture();
	return _screenshot;
}

void QtGrabber::start()
{
	_timer.start();
}

void QtGrabber::stop()
{
	_timer.stop();
}

void QtGrabber::capture()
{
	_screen = QGuiApplication::primaryScreen();
	if (!_screen)
		return;

	QPixmap originalPixmap = _screen->grabWindow(0);
	QPixmap scaledPixmap = originalPixmap.scaled(_width,_height);
	QImage img = scaledPixmap.toImage();
	img = img.convertToFormat( QImage::Format_RGB888);
	memcpy(_screenshot.memptr(), img.bits(),_width*_height*3);

	emit sig_screenshot(_screenshot);
}


void QtGrabber::closeEvent(QCloseEvent *event)
{
	event->ignore();
}
