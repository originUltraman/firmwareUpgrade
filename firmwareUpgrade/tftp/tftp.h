#ifndef TFTP_H
#define TFTP_H
#include<QObject>
#include<QFile>
#include<QUdpSocket>
#include "dir.h"
//#define TFTP_NOT_LAST_DATA_LEN 512
//#define MAX_RETRANS_TIMES 20
//#define WAIT_TIME_MS 2000

class Tftp
{
public:
    Tftp();
    enum TftpPacketType{RRQ = 1, WRQ, DATA, ACK, ERROR, OACK, UNDEFINED};
    //static bool sendFile(QUdpSocket *uSock, QString fileName, QString* errorMessage, bool* mainThreadExitedOrNot, enum TftpMode tftpMode = RRQ);
    //static bool sendFile_GBN(QUdpSocket *uSock, QString fileName, QString* errorMessage, bool* mainThreadExitedOrNot, enum TftpMode tftpMode = RRQ);
    //static bool receiveFile(QUdpSocket *uSock, QString fileName, QString* errorMessage, bool* mainThreadExitedOrNot, enum TftpMode tftpMode = RRQ);


    static void parseOACK(QByteArray &oackData);
    static bool put(QUdpSocket* uSock, QString path, QString fileName, QString* errorMessage, const QHostAddress& host, quint16 port);
    static bool get(QUdpSocket* uSock, QString path, QString fileName, QString* errorMessage, const QHostAddress& host, quint16 port);
    static bool handlePut(QUdpSocket* uSock, QString path, QString fileName, QString* errorMessage, const QHostAddress& host, const quint16 port, QByteArray writeRequest);
    static bool handleGet(QUdpSocket* uSock, QString path, QString fileName, QString* errorMessage, const QHostAddress& host, const quint16 port, QByteArray readRequest);
    static bool download(QUdpSocket* uSock, QString path, QString fileName, QString* errorMessage, const QHostAddress& host, const quint16 port, unsigned short blksize, unsigned short timeout, unsigned short maxRetransmit,QByteArray& lastPacket);
    static bool upload(QUdpSocket* uSock, QString path, QString fileName, QString* errorMessage, const QHostAddress& host, const quint16 port, unsigned short blksize, unsigned short timeout, unsigned short maxRetransmit);

    static QByteArray makeTftpData(char data[], int len, quint16 block);
    static char* makeTftpData(char data[], quint16 block);
    static QByteArray makeTftpReadRequest(QString fileName, quint16 valueOfBlockSize, quint16 valueOfTimeOut, quint16 maxRetransmit, QString mode = "octet");
    static QByteArray makeTftpWriteRequest(QString fileName, quint16 valueOfBlockSize, quint16 valueOfTimeOut, quint16 maxRetransmit, QString mode = "octet");
    static QByteArray makeTftpAck(quint16 block);
    static QByteArray makeTftpOAck(std::initializer_list<std::pair<std::string, std::string>> options);
    static QByteArray makeTftpError(quint16 errorCode, QString errorMessage);
    static QByteArray makeTftpAbort(unsigned short statusCode);

    static TftpPacketType getTftpPacketType(const QByteArray& tftpPacket);
    static bool checkBlockNo(const QByteArray& tftpPacket, unsigned short No);
    static bool checkAckNo(const QByteArray& tftpPacket, unsigned short No);
    static unsigned short getNo(const QByteArray& tftpPacket);
};
#endif // TFTP_H

