
// QT includes
#include <QTimer>
#include <QScreen>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QWidget>

#include <utils/ColorRgb.h>
#include <utils/Image.h>
#include "JsonConnection.h"
#include <vector>
#include <QColorDialog>
#include <QCloseEvent>

class QtGrabber : public QWidget
{
	Q_OBJECT

public:
	QtGrabber(const unsigned display, const unsigned grabWidth, const unsigned grabHeight, const unsigned updateRate_Hz);
	~QtGrabber();

	const Image<ColorRgb> & getScreenshot();

public slots:
	void start();
	void stop();
	void setXColor();
	void newColor(const QColor & color);
	void closeEvent(QCloseEvent *event);
 
signals:
	void sig_screenshot(const Image<ColorRgb> & screenshot);

private slots:
	///
	/// Performs a single screenshot capture and publishes the capture screenshot on the screenshot signal.
	///
	void capture();
	void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
	void createTrayIcon();

	/// The QT timer to generate capture-publish events
	QTimer _timer;
	QAction *quitAction;
	QAction *startAction;
	QAction *stopAction;
	QAction *colorAction;

	int _width;
	int _height;
	// image buffers
	Image<ColorRgb>  _screenshot;
	QScreen *_screen;

    QSystemTrayIcon *_trayIcon;
    QMenu *_trayIconMenu;
	JsonConnection* json;
	QColorDialog _colorDlg;
};
