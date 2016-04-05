#pragma once

#include "Messages.pb.h"
#include "IConnection.h"


namespace TestbeamCommunication
{
//class IConnection;
class IMessageHandler
{
public:
    virtual ~IMessageHandler(){}
    virtual void handleMsg(const tutorial::TestbeamMessage& msg) = 0;
};

class MessageHandler: public IMessageHandler
{
public:
    MessageHandler(    IConnection::ConnectionPointer & mambaClient,
                       IConnection::ConnectionPointer & telescopeClient);
    virtual ~MessageHandler(){}
    virtual void handleMsg(const tutorial::TestbeamMessage& msg) = 0;

protected:
    IConnection::ConnectionPointer& m_mambaClient;
    IConnection::ConnectionPointer& m_telescopeClient;
};

class StartEventMambaHandlder : public MessageHandler
{
public:
    StartEventMambaHandlder(    IConnection::ConnectionPointer& mambaClient,
                       IConnection::ConnectionPointer& telescopeClient);

    virtual void handleMsg(const tutorial::TestbeamMessage& msg);
private:
	void logMsg(tutorial::StartEventReq& startEventReq);
    IConnection::MessagePointer prepareResponse(long seqNo,tutorial::StartEventReq* startEventReq );
};

class StartEventTelescopeHandlder : public MessageHandler
{
public:
    StartEventTelescopeHandlder(    IConnection::ConnectionPointer & mambaClient,
                       IConnection::ConnectionPointer & telescopeClient);

    virtual void handleMsg(const tutorial::TestbeamMessage & msg);
private:
    IConnection::MessagePointer prepareResponse(long seqNo, tutorial::StartEventResp* startEventReq );
};

class StopEventMambaHandlder : public MessageHandler
{
public:
    StopEventMambaHandlder(    IConnection::ConnectionPointer & mambaClient,
                       IConnection::ConnectionPointer & telescopeClient);

    virtual void handleMsg(const tutorial::TestbeamMessage & msg);
private:
    IConnection::MessagePointer prepareResponse(long seqNo );

};

class StopEventTelescopeHandlder : public MessageHandler
{
public:
    StopEventTelescopeHandlder(    IConnection::ConnectionPointer & mambaClient,
                       IConnection::ConnectionPointer & telescopeClient);

    virtual void handleMsg(const tutorial::TestbeamMessage & msgToSend);
private:

};


}
