#include "thread.h"

thread::thread(const Device *device, TftpRequest *tftpRequest, QObject *parent):
    QObject(nullptr),
    device(device),
    tftpRequest(tftpRequest){
    //创建一个虚拟连接，只接受目的设备端发来的信息
    QDir tmpDir(QDir::currentPath() + "/" + device->getName() + "_" + device->getHostAddress());
    if(!tmpDir.exists()){
        qDebug() << QDir::currentPath();
        if(!tmpDir.mkpath(QDir::currentPath() + "/" + device->getName() + "_" + device->getHostAddress())) qDebug() << "文件夹创建失败";
        else qDebug() << "文件创建成功";
    }
    dir = tmpDir;
}

thread::~thread(){
    if(tftpRequest) delete tftpRequest;
    qDebug() << "MyThread的析构函数\n";
}

const QHostAddress thread::getHostAddress() const{
    return QHostAddress(this->device->getHostAddress());
}

TftpRequest *thread::getTftpRequest() const{
    return tftpRequest;
}

const Device *thread::getDevice() const{
    return device;
}

void thread::waitStatusFileRcved()
{
    conditionMutex.lock();
    while(statusFileRcved == false){
        statusFileRcvedConditon.wait(&conditionMutex);
    }
    conditionMutex.unlock();
}

bool thread::waitStatusFileRcved(QString& errorMessage, unsigned long mseconds)
{
    conditionMutex.lock();
    bool noTimeOut = true;
    while(noTimeOut == true && statusFileRcved == false){
        //超时了会返回false
        noTimeOut = statusFileRcvedConditon.wait(&conditionMutex, mseconds);
    }
    statusFileRcved = false;
    conditionMutex.unlock();
    if(noTimeOut == false) errorMessage = QString("等待状态文件超时");
    return noTimeOut;
}

void thread::mainThreadExited(){
    QMutexLocker locker(&mutex);
    mainThreadExitedOrNot = true;
}