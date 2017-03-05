
// Hyperion-AmLogic includes
#include "qt_grabber.h"
#include <QPixmap>
#include <QWindow>
#include <QGuiApplication>
#include <QWidget>

QtGrabber::QtGrabber(const unsigned display, const unsigned grabWidth, const unsigned grabHeight, const unsigned updateRate_Hz)
	: _timer(this)
	, _width(grabWidth)
	, _height(grabHeight)
	, _screenshot(grabWidth, grabHeight)
{
	_timer.setSingleShot(false);
	_timer.setInterval(updateRate_Hz);

	// Connect capturing to the timeout signal of the timer
	connect(&_timer, SIGNAL(timeout()), this, SLOT(capture()));
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
