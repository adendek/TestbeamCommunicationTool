#include "TestbeamConnection.h"
#include "MessageHandler.h"
#include <iostream>

#define DEBUG true

using namespace TestbeamCommunication;
using namespace std;
namespace asio = boost::asio;
using asio::ip::tcp;
using boost::uint8_t;


IConnection::DeviceType TestbeamConnection::getType()
{
  IConnection::MessagePointer initReq;
  initReq->set_msgtype(tutorial::TestbeamMessage_MESSAGE_TYPE_INIT_REQ);
  initReq->set_seqno(1);
  sendMsg(initReq);
  start_read_header();
  return m_devType;
}

void TestbeamConnection::sendMsg(IConnection::MessagePointer& msgToSend)
{
    vector<uint8_t> writebuf;
    PackedMessage<tutorial::TestbeamMessage> resp_msg(msgToSend);
    resp_msg.pack(writebuf);
    asio::write(m_socket, asio::buffer(writebuf));
}

void TestbeamConnection::handle_read_header(const boost::system::error_code& error)
{

    std::cout<< "handle read " << error.message() << endl;
    if (!error) {
        cout<< "Got header!\n"<< endl;
        cout<<  show_hex(m_readbuf) << endl;
        unsigned msg_len = m_packed_message.decode_header(m_readbuf);
         cout<< msg_len << " bytes"<< endl;
        start_read_body(msg_len);
    }
}

void TestbeamConnection::handle_read_body(const boost::system::error_code& error)
{
    if(DEBUG) (cerr << "handle body " << error << '\n');
    if (!error) {
        if(DEBUG)  (cerr << "Got body!\n");
        if(DEBUG)  (cerr << show_hex(m_readbuf) << endl);
        if(IConnection::DeviceType::UNKNOWN != m_devType){
            start_read_header();
            handle_message();
        }
        else handle_init();
    }
}

void TestbeamConnection::start_read_header()
{
    std::cout<<"Connection---> satrt read header"<<std::endl;

    m_readbuf.resize(HEADER_SIZE);
    std::cout<<"Connection---> after resize"<<std::endl;
    asio::async_read(m_socket, asio::buffer(m_readbuf),
            boost::bind(&TestbeamConnection::handle_read_header, shared_from_this(),
                asio::placeholders::error));
    std::cout<<"Connection---> after async read"<<std::endl;

}


void TestbeamConnection::handle_message()
{
    if (m_packed_message.unpack(m_readbuf)) {
        MessagePointer req = m_packed_message.get_msg();
        if(m_msgHandlersMap.end() == m_msgHandlersMap.find(req->msgtype())){
            cout<<"wrong type of message"<<endl;
            return;
        }
        m_msgHandlersMap[req->msgtype()]->handleMsg(*req);
    }
}

void TestbeamConnection::start_read_body(unsigned msg_len)
{
    m_readbuf.resize(HEADER_SIZE + msg_len);
    asio::mutable_buffers_1 buf = asio::buffer(&m_readbuf[HEADER_SIZE], msg_len);
    asio::async_read(m_socket, buf,
            boost::bind(&TestbeamConnection::handle_read_body, shared_from_this(),
                asio::placeholders::error));
}



void TestbeamConnection::handle_init()
{
    if (m_packed_message.unpack(m_readbuf)) {
        IConnection::MessagePointer req = m_packed_message.get_msg();
        if(tutorial::TestbeamMessage_MESSAGE_TYPE_INIT_RESP == req->msgtype()){
            tutorial::InitDevice devTypeStruct = req->initdevice();
            switch(devTypeStruct.devicetype())
            {
            case tutorial::InitDevice_DeviceType_MAMBA:
                m_devType=IConnection::DeviceType::MAMBA;
                break;
            case tutorial::InitDevice_DeviceType_TELESCOPE:
                m_devType=IConnection::DeviceType::TELESCOPE;
                break;
           default:
                cout<<"wrong message type"<<endl;
                break;
            }

        }else
            cout<<"Received wrong answer!"<<endl;

    }
}


