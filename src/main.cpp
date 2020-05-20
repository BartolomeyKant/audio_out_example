#include <iostream>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "src/controller.h"

using namespace std;

int main(int argc, char** argv){

	cout << "Luntink was borned!" << endl;

	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;

	Controller *_controller = new Controller(&engine);

	qmlRegisterSingletonInstance<Controller>("my.company.controller", 1,0,"Controller", _controller);

	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(
	    &engine,
	    &QQmlApplicationEngine::objectCreated,
	    &app,
	    [url](QObject* obj, const QUrl& objUrl) {
		    if (!obj && url == objUrl)
			    QCoreApplication::exit(-1);
	    },
	    Qt::QueuedConnection);
	engine.load(url);
	int res = app.exec();
	return res;
}
