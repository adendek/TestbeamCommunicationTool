#include "MessageHandler.h"
#include <iostream>


using namespace std;
using namespace TestbeamCommunication;


MessageHandler::MessageHandler(    IConnection::ConnectionPointer & mambaClient,
                                       IConnection::ConnectionPointer & telescopeClient):
    m_mambaClient(mambaClient),
    m_telescopeClient(telescopeClient)
{
    cout<<"MessageHander  has been created"<<endl;
}

StartEventMambaHandlder::StartEventMambaHandlder(IConnection::ConnectionPointer& mambaClient,
                                   IConnection::ConnectionPointer& telescopeClient):
    MessageHandler(mambaClient,telescopeClient)
{
    cout<<"StartEventMambaHandlder  has been created"<<endl;
}

void StartEventMambaHandlder::handleMsg(const tutorial::TestbeamMessage& msg)
{
    cout<<"received TestbeamMessage_MESSAGE_TYPE_START_EVENT_MAMBA_REQ"<<endl;
    cout<<"sequence number!"<< msg.seqno()<<endl;
    auto startEventReq = msg.starteventreq();
    logMsg(startEventReq);
    IConnection::MessagePointer telescopeMsg=prepareResponse( msg.seqno(),&startEventReq);
    MessageHandler::m_telescopeClient->sendMsg(telescopeMsg);
}


void StartEventMambaHandlder::logMsg(tutorial::StartEventReq& startEventReq)
{
    cout<<"print parameters!"<<endl;
    cout<< "runtype" << startEventReq.runtype() <<endl;
    cout<< "biasvoltage" << startEventReq.biasvoltage() <<endl;
    cout<< "angle" << startEventReq.angle() <<endl;
    cout<< "stagex" << startEventReq.stagex() <<endl;
    cout<< "stagey" << startEventReq.stagey() <<endl;
    cout<< "temperature" << startEventReq.temperature() <<endl;
    cout<< "sector" << startEventReq.sector() <<endl;
    cout<< "sensorname" << startEventReq.sensorname() <<endl;
    cout<< "operatorname" << startEventReq.operatorname() <<endl;
    cout<< "****************************************************" <<endl;
}

IConnection::MessagePointer StartEventMambaHandlder::prepareResponse(long seqNo,tutorial::StartEventReq* startEventReq )
{
    IConnection::MessagePointer telescopeMsg;
    telescopeMsg->set_msgtype(tutorial::TestbeamMessage_MESSAGE_TYPE_START_EVENT_TIMEPIX_REQ);
    telescopeMsg->set_seqno(seqNo);
    telescopeMsg->mutable_starteventreq()->Swap(startEventReq);
    return telescopeMsg;
}


/*/
 *
 * StartEventTelescopeHandlder
 *
 */


StartEventTelescopeHandlder::StartEventTelescopeHandlder(IConnection::ConnectionPointer& mambaClient,
                                   IConnection::ConnectionPointer& telescopeClient):
    MessageHandler(mambaClient,telescopeClient)
{
    cout<<"StartEventTelescopeHandlder  has been created"<<endl;
}


void StartEventTelescopeHandlder::handleMsg(const tutorial::TestbeamMessage& msg)
{
    cout<<"received TestbeamMessage_MESSAGE_TYPE_START_EVENT_TIMEPIX_RESP"<<endl;
    cout<<"sequence number!"<< msg.seqno()<<endl;
    auto resp =  msg.starteventresp();
    cout<<"kepler run number " << resp.keplerrunnumber()<<endl;
    IConnection::MessagePointer mambaMsg = prepareResponse(msg.seqno(), &resp);
    MessageHandler::m_mambaClient->sendMsg(mambaMsg);
}

IConnection::MessagePointer StartEventTelescopeHandlder::prepareResponse(long seqNo,tutorial::StartEventResp* startEventResp )
{
    IConnection::MessagePointer mambaMsg;
    mambaMsg->set_msgtype(tutorial::TestbeamMessage_MESSAGE_TYPE_START_EVENT_MAMBA_RESP);
    mambaMsg->set_seqno(seqNo);
    mambaMsg->mutable_starteventresp()->Swap(startEventResp);
    return mambaMsg;
}

/*
 *
 * StopEventMambaHandlder
 *
 */
 StopEventMambaHandlder::StopEventMambaHandlder(IConnection::ConnectionPointer& mambaClient,
                                   IConnection::ConnectionPointer& telescopeClient):
    MessageHandler(mambaClient,telescopeClient)
{
    cout<<"StopEventMambaHandlder  has been created"<<endl;
}


void StopEventMambaHandlder::handleMsg(const tutorial::TestbeamMessage& msg)
{
    cout<<"received TestbeamMessage_MESSAGE_TYPE_START_EVENT_TIMEPIX_RESP"<<endl;
    cout<<"sequence number!"<< msg.seqno()<<endl;
    cout<<"run is good " << msg.stopeventreq().isgood()<<endl;
    IConnection::MessagePointer mambaMsg = prepareResponse(msg.seqno());
    MessageHandler::m_mambaClient->sendMsg(mambaMsg);
}

IConnection::MessagePointer StopEventMambaHandlder::prepareResponse(long seqNo )
{
    IConnection::MessagePointer telescopeMsg;
    telescopeMsg->set_msgtype(tutorial::TestbeamMessage_MESSAGE_TYPE_STOP_EVENT_TIMEPIX_REQ);
    telescopeMsg->set_seqno(seqNo);
    return telescopeMsg;
}

/*
 *
 * StopEventTelescopeHandlder
 *
 */

StopEventTelescopeHandlder::StopEventTelescopeHandlder(IConnection::ConnectionPointer& mambaClient,
                                   IConnection::ConnectionPointer& telescopeClient):
    MessageHandler(mambaClient,telescopeClient)
{
    cout<<"StopEventTelescopeHandlder  has been created"<<endl;
}


void StopEventTelescopeHandlder::handleMsg(const tutorial::TestbeamMessage& msg)
{
    cout<<"received TestbeamMessage_MESSAGE_TYPE_STOP_EVENT_TIMEPIX_RESP"<<endl;
    cout<<"sequence number!"<< msg.seqno()<<endl;
    // do nothing!
}


