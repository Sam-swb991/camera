#include "blockchaindata.h"

blockchainData::blockchainData()
{

}

int blockchainData::getAlarm()
{
    return Alarm;
}

int blockchainData::getSensorstatus()
{
    return SensorStatus;
}

int blockchainData::getStatus()
{
    return Status;
}

string blockchainData::getData()
{
    return Data;
}

string blockchainData::getDevName()
{
    return devName;
}

string blockchainData::getHash()
{
    return hash;
}

string blockchainData::getIP()
{
    return IP;
}

string blockchainData::getPrehash()
{
    return prehash;
}

string blockchainData::getSN()
{
    return SN;
}

string blockchainData::getTime()
{
    return time;
}

void blockchainData::setAlarm(int alarm)
{
    this->Alarm = alarm;
}

void blockchainData::setData(string data)
{
    this->Data.clear();
    this->Data.append(data);
}

void blockchainData::setDevName(string devname)
{
    this->devName.clear();
    this->devName.append(devname);
}

void blockchainData::setHash(string hash)
{
    this->hash.clear();
    this->hash.append(hash);
}

void blockchainData::setIP(string ip)
{
    this->IP.clear();
    this->IP.append(ip);
}

void blockchainData::setPrehash(string prehash)
{
    this->prehash.clear();
    this->prehash.append(prehash);
}

void blockchainData::setSN(string sn)
{
    this->SN.clear();
    this->SN.append(sn);
}

void blockchainData::setSensorStatus(int sensorstatus)
{
    this->SensorStatus = sensorstatus;
}

void blockchainData::setStatus(int status)
{
    this->Status = status;
}

void blockchainData::setTime(string time)
{
    this->time.clear();
    this->time.append(time);
}
