#pragma once

#include <memory>
#include <boost/asio.hpp>

namespace TestbeamCommunication
{
class CommunicationMgr 
{
public:
    CommunicationMgr() = delete;
    CommunicationMgr(boost::asio::io_service& io_service, unsigned port);
    ~CommunicationMgr();

private:
    void startAccept();

    struct CommunicationMgrImpl; 
    std::unique_ptr<CommunicationMgrImpl> m_communicationMgrImplPtr;
};
}

