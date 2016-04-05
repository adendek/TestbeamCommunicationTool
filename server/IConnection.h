#pragma once
#include "Messages.pb.h"
#include <map>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>


namespace TestbeamCommunication
{
class IMessageHandler;

class IConnection
{
public:
    typedef boost::shared_ptr<IConnection> ConnectionPointer;
    typedef boost::shared_ptr<tutorial::TestbeamMessage> MessagePointer;
    enum DeviceType{
        UNKNOWN = 0,
        MAMBA = 1,
        TELESCOPE =2
    };

    virtual ~IConnection(){}
    virtual void start() = 0;
    virtual boost::asio::ip::tcp::socket& get_socket() =0 ;
    virtual void sendMsg(IConnection::MessagePointer& msgToSend) = 0;
    virtual DeviceType getType() = 0;
    virtual void setHandlerMap(std::map<tutorial::TestbeamMessage_MESSAGE_TYPE, boost::shared_ptr<IMessageHandler> >& handersMap) = 0;
};
}
