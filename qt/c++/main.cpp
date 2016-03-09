/*****************************************************************************
THIS IS A CODE SAMPLE.

##############################
# (C) 2015 Juhapekka Piiroinen
# All Rights Reserved.
##############################
# Author(s):
#   Juhapekka Piiroinen <juhapekka.piiroinen@1337.fi>
##############################
******************************************************************************/

#include <QApplication>
#include <QtQml>

//Q_IMPORT_PLUGIN(cuteplugins)

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationDisplayName("Cute App Launcher");
    app.setApplicationVersion("0.1");
    app.setApplicationName("cuteapplauncher");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("app",&app);
    engine.addImportPath(app.applicationDirPath() + "/imports");

    QPluginLoader loader("libs/features/libcuteplugins.so");
    if (loader.load()) {
        QQmlExtensionPlugin* plugin = qobject_cast<QQmlExtensionPlugin*>(loader.instance());
        if (plugin) {
            plugin->registerTypes("fi.leetsoftware.cute");
            plugin->initializeEngine(&engine,"fi.leetsoftware.cute");
        }
        return app.exec();
    } else {
        qDebug() << loader.errorString();
    }

    return -2;
}
