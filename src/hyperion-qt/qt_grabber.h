
// QT includes
#include <QTimer>
#include <QScreen>

#include <utils/ColorRgb.h>
#include <utils/Image.h>

class QtGrabber : public QObject
{
	Q_OBJECT
public:
	QtGrabber(const unsigned display, const unsigned grabWidth, const unsigned grabHeight, const unsigned updateRate_Hz);

	const Image<ColorRgb> & getScreenshot();

	///
	/// Starts the timed capturing of screenshots
	///
	void start();

	void stop();

signals:
	void sig_screenshot(const Image<ColorRgb> & screenshot);

private slots:
	///
	/// Performs a single screenshot capture and publishes the capture screenshot on the screenshot signal.
	///
	void capture();

private:
	/// The QT timer to generate capture-publish events
	QTimer _timer;

	int _width;
	int _height;
	// image buffers
	Image<ColorRgb>  _screenshot;
	QScreen *_screen;

	
};
