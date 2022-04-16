#include "StockfishEngine.h"

#include <QDebug>

namespace helper
{
    QString toFen(const ChessBoard& board) {
        return QString();
    };

    ChessBoard fromFen(const QString& fen) {
        return ChessBoard();
    }
};

template< typename T, class ... Args >
static QString GenerateCommand(Args&&... args) {
    return T::MakeCommand(std::forward<Args>(args)...);
}

template< class ... Args >
static QString Wrap(const Args&... args)
{
    QStringList command = {args..., "\n"};

    return command.join(" ");
}

struct InitUci {
    static QString MakeCommand(const QString& fen) {
        return Wrap("uci");
    }
};

struct IsReady {
    static QString MakeCommand() {
        return Wrap("isready");
    }
};

struct NewGame {
    static QString MakeCommand(const QString& fen) {
        return Wrap("ucinewgame");
    }
};

struct SetPosition {
    static QString MakeCommand(const QString& fen = "") {
        if(!fen.isEmpty())
            return Wrap("position", fen);
        else
            return Wrap("startpos");
    }
};

struct Compute {
    static QString MakeCommand() {
        return Wrap("go");
    }
};

struct StopCompute {
    static QString MakeCommand() {
        return Wrap("stop");
    }
};

struct Quit {
    static QString MakeCommand() {
        return Wrap("quit");
    }
};

StockfishEngine::StockfishEngine(QObject* parent) : QObject(parent),
    m_process()
{
    m_process.setProgram("../engines/stockfish");


    // QObject::connect(this, &QProcess::started, []() { qDebug() << "program started"; });
    // QObject::connect(this, &QProcess::finished, [](int exitCode, QProcess::ExitStatus exitStatus) { qDebug() << "program finished(" << exitCode << ")"; });
    // QObject::connect(this, &QProcess::errorOccurred, [](QProcess::ProcessError error) { qDebug() << "error occurred !"; });
    // QObject::connect(this, &QProcess::stateChanged, [](QProcess::ProcessState newState) { qDebug() << newState; });

    QObject::connect(&m_process, &QProcess::readyReadStandardOutput, this, &StockfishEngine::processStandardOutput);
    QObject::connect(&m_process, &QProcess::readyReadStandardError,  this, &StockfishEngine::processStandardError);
}

void StockfishEngine::start()
{
    if(m_process.state() != QProcess::ProcessState::NotRunning)
        throw std::runtime_error("Do not call 'start()'. Stockfish has already start");

    m_process.start();

    if(!m_process.waitForStarted(10000))
        throw std::runtime_error("Stockfish should have started before timeout. Something goes wrong");
}

void StockfishEngine::stop()
{
    if(m_process.state() == QProcess::ProcessState::NotRunning)
        throw std::runtime_error("Do not call 'stop()'. Stockfish is not running");

    m_process.write("quit\n");

    if(!m_process.waitForFinished(10000))
        throw std::runtime_error("Stockfish cannot stop. Something goes wrong");
}

void StockfishEngine::setPosition(const ChessBoard& board)
{
    QString fen = helper::toFen(board);

    QString command = GenerateCommand<SetPosition>(fen);

    send(command);
}

void StockfishEngine::compute()
{
    QString command = GenerateCommand<Compute>();

    send(command);
}

void StockfishEngine::send(const QString& command)
{
    const char* buffer = command.toStdString().c_str();

    m_process.write(buffer);
}

void StockfishEngine::processStandardOutput()
{
    QByteArray buffer = m_process.readAllStandardOutput();
    qDebug() << buffer;
}

void StockfishEngine::processStandardError()
{
    QByteArray buffer = m_process.readAllStandardError();
    qDebug() << buffer;
}
