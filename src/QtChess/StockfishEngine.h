#ifndef QTCHESS_STOCKFISHENGINE_H
#define QTCHESS_STOCKFISHENGINE_H

// QtChess
#include <QtChess/ChessBoard.h>

// Qt
#include <QProcess>
#include <QString>

class StockfishEngine : public QObject
{
    Q_OBJECT

public:
    StockfishEngine(QObject* parent = nullptr);

public:
    void start();
    void stop();

public:
    void setPosition(const ChessBoard& board);
    void compute();

private:
    void send(const QString& command);

    void processStandardOutput();
    void processStandardError();

private:
    QProcess m_process;

};

#endif // QTCHESS_STOCKFISHENGINE_H
