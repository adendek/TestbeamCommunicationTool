#include "CommunicationMgr.h"
#include "TestbeamConnection.h"
#include "PackedMessage.h"
#include "MessageHandler.h"
#include "Messages.pb.h"
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/cstdint.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace TestbeamCommunication;
using namespace std;
namespace asio = boost::asio;
using asio::ip::tcp;
using boost::uint8_t;


struct CommunicationMgr::CommunicationMgrImpl
{
    tcp::acceptor acceptor;
    CommunicationMgrImpl(asio::io_service& io_service, unsigned port)
        : acceptor(io_service, tcp::endpoint(tcp::v4(), port))
    {
        initMessageHandlers();
        start_accept();
    }
     void initMessageHandlers();

    void start_accept()
    {
        TestbeamConnection::ConnectionPointer new_connection =
            TestbeamConnection::create(acceptor.get_io_service(),m_handlersMap);

        acceptor.async_accept(new_connection->get_socket(),
            boost::bind(&CommunicationMgrImpl::handle_accept, this, new_connection,
                asio::placeholders::error));
    }

    void handle_accept(TestbeamConnection::ConnectionPointer connection,
            const boost::system::error_code& error)
    {
        cout<<"new client has been connected"<<endl;
        if (!error) {
            IConnection::DeviceType deviceType = connection->getType();
            if(deviceType == IConnection::DeviceType::MAMBA){
                cout<<"client is mamba"<<endl;
                m_mambaPtr=connection;
            }else{
                cout<<"client is telescope"<<endl;
                m_telescopePtr=connection;
            }
            connection->start();
            if(!m_mambaPtr || !m_telescopePtr )start_accept();
        }
    }
private:
TestbeamConnection::ConnectionPointer  m_mambaPtr;
TestbeamConnection::ConnectionPointer m_telescopePtr;
std::map<tutorial::TestbeamMessage_MESSAGE_TYPE, boost::shared_ptr<IMessageHandler> > m_handlersMap;


};

void CommunicationMgr::CommunicationMgrImpl::initMessageHandlers()
{
    m_handlersMap={
        {tutorial::TestbeamMessage_MESSAGE_TYPE_START_EVENT_MAMBA_REQ, boost::shared_ptr<IMessageHandler>(new StartEventMambaHandlder(m_mambaPtr,m_telescopePtr)) },
        {tutorial::TestbeamMessage_MESSAGE_TYPE_START_EVENT_TIMEPIX_RESP, boost::shared_ptr<IMessageHandler>(new StartEventTelescopeHandlder(m_mambaPtr,m_telescopePtr)) },
        {tutorial::TestbeamMessage_MESSAGE_TYPE_STOP_EVENT_MAMBA_REQ, boost::shared_ptr<IMessageHandler>(new StopEventMambaHandlder(m_mambaPtr,m_telescopePtr)) },
        {tutorial::TestbeamMessage_MESSAGE_TYPE_STOP_EVENT_TIMEPIX_RESP, boost::shared_ptr<IMessageHandler>(new StopEventTelescopeHandlder(m_mambaPtr,m_telescopePtr)) }
    };
}

CommunicationMgr::CommunicationMgr(asio::io_service& io_service, unsigned port)
    : m_communicationMgrImplPtr(new CommunicationMgrImpl(io_service, port))
{
}


CommunicationMgr::~CommunicationMgr()
{
}


