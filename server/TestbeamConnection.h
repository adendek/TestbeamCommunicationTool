#pragma once

#include "PackedMessage.h"
#include "Messages.pb.h"
#include "IConnection.h"
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/cstdint.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace TestbeamCommunication
{

class TestbeamConnection : public IConnection , public boost::enable_shared_from_this<TestbeamConnection>
{
public:
    static IConnection::ConnectionPointer create(boost::asio::io_service& io_service,
                                    std::map<tutorial::TestbeamMessage_MESSAGE_TYPE, boost::shared_ptr<IMessageHandler> >& handlersMap)
    {
        return IConnection::ConnectionPointer(new TestbeamConnection(io_service, handlersMap));
    }

    boost::asio::ip::tcp::socket& get_socket()
    {
        return m_socket;
    }

    void start()
    {
        std::cout<<"Connection---> satart"<<std::endl;
        start_read_header();
    }

    virtual void setHandlerMap(std::map<tutorial::TestbeamMessage_MESSAGE_TYPE, boost::shared_ptr<IMessageHandler> >& handlersMap){
        m_msgHandlersMap=handlersMap;
    }

    void sendMsg(IConnection::MessagePointer& msgToSend);
    IConnection::DeviceType getType();

private:
    TestbeamConnection(boost::asio::io_service& io_service,
                       std::map<tutorial::TestbeamMessage_MESSAGE_TYPE, boost::shared_ptr<IMessageHandler> >& msgHandlersMap)
        : m_devType(IConnection::DeviceType::UNKNOWN),
          m_socket(io_service),
          m_packed_message(boost::shared_ptr<tutorial::TestbeamMessage>(new tutorial::TestbeamMessage())),
          m_msgHandlersMap(msgHandlersMap)
    {
    }
    void handle_read_header(const boost::system::error_code& error);
    void handle_read_body(const boost::system::error_code& error);
    void handle_message();
    void start_read_header();
    void start_read_body(unsigned msg_len);
    void handle_init();


    IConnection::DeviceType m_devType;
    boost::asio::ip::tcp::socket m_socket;
    std::vector<uint8_t> m_readbuf;
    PackedMessage<tutorial::TestbeamMessage> m_packed_message; 
    std::map<tutorial::TestbeamMessage_MESSAGE_TYPE, boost::shared_ptr<IMessageHandler> >& m_msgHandlersMap;



};

}
