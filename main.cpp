#include <iostream>

#include <QtChess/StockfishEngine.h>

// Qt
#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QTimer>

int main(int argc, char* argv[])
{
    // return 0;
    QCoreApplication app(argc, argv);
    QProcess process;

    QObject::connect(&process, &QProcess::started, []() { qDebug() << "program started"; });
    QObject::connect(&process, &QProcess::errorOccurred, [](QProcess::ProcessError error) { qDebug() << "error occurred !"; });
    // QObject::connect(&process, &QProcess::finished, [](int exitCode, QProcess::ExitStatus exitStatus) { qDebug() << "program finished(" << exitCode << ")"; });
    QObject::connect(&process, &QProcess::stateChanged, [](QProcess::ProcessState newState) { qDebug() << newState; });

    QObject::connect(&process, &QProcess::readyReadStandardError, [&process]() {
        QString errors = process.readAllStandardOutput();
        qDebug() << errors;
    });

    QObject::connect(&process, &QProcess::readyReadStandardOutput, [&process]() {
        QString outputs = process.readAllStandardOutput();
        qDebug() << "->";
        qDebug() << outputs;
    });

    {
        QString program = "../engines/stockfish";

        process.setProgram(program);
        qDebug() << "start program";
        process.start();

        if(!process.waitForStarted())
            return 49;
    }

    process.write("isready\n");
    // process.write("quit");

    // pr
    // QQuickView view;

    // QQmlEngine* engine = view.engine();
    // QObject::connect(engine, &QQmlEngine::quit, []() {
    //     qDebug() << "exit";
    // });

    // {
    //     view.setSource(QUrl::fromLocalFile("../main.qml"));
    //     view.show();
    // }

    QTimer::singleShot(1000, [&process]() {
        process.write("go depth 3\n");
    });

    QTimer::singleShot(3000, [&process]() {
        qDebug() << "trigger";

        process.write("quit\n");

        process.waitForFinished();

        qApp->quit();
    });

    return app.exec();
}
