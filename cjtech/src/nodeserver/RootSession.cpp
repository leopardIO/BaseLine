/*************************************************************************
	> File Name: HeadStructMessage.h
	> Author: zhonghx
	> Mail: zhonghongxia@foxmail.com
	> Created Time: Wed 26 Aug 2015 08:30:05 PM UTC
 ************************************************************************/

#include "RootSession.h"
#include "ClientSession.h"
#include <glog/logging.h>  
#include <glog/raw_logging.h>  

using std::cout;
using std::cin;
using std::endl;
using boost::asio::ip::tcp;

extern NodeServer::IOServicePool *g_io_service_pool;
extern NodeServer::SessionManager* g_session_manager; 
namespace NodeServer
{
	RootSession::~RootSession()
	{
		if(_p_socket_temp_ != NULL)
        {
			delete _p_socket_temp_;
        }
	}
	RootSession::RootSession(short port)
		: _acceptor_(g_io_service_pool->GetIoService(), tcp::endpoint(tcp::v4(), port))		
	{
			
	}
	void RootSession::Run()
	{
        LOG(INFO) << "RootSession :: Root Session is Runing Now";  
		_p_socket_temp_ = new tcp::socket(g_io_service_pool->GetIoService());
		_acceptor_.async_accept(*_p_socket_temp_,
				boost::bind(&RootSession::H_New_Session, this,  
					boost::asio::placeholders::error));				
	}
	void RootSession::H_New_Session(const boost::system::error_code& error)
	{
        if(!error)
        {
            LOG(INFO)<< "RootSession :: New connection from Root "
                <<_p_socket_temp_->remote_endpoint().address().to_string();
		    ClientSession * client_session = g_session_manager->CreateSession<ClientSession >(_p_socket_temp_);
            client_session->Start();
            Run() ;

        }

		else
		{
#ifdef DEBUG
            LOG(ERROR)<<"RootSession :: read proto error :" <<error.message();
#endif
			g_session_manager->Recycle(GetSessionID());
		}

	}

}

