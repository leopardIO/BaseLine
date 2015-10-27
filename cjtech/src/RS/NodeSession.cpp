#include "NodeSession.h"
#include <iostream>
#include<stdlib.h>
#include <glog/logging.h>
#include <glog/raw_logging.h>
#include "../../common/includeopencv/interface.h"
#include "NodeMessage.h"
#include "SessionService.h"

using namespace std;

namespace cjtech
{
    namespace RootServer
    {
        NodeSession::NodeSession(boost::asio::io_service& io_service,
                tcp::resolver::iterator endpoint_iterator):
            _socket_(io_service)
        {
            boost::asio::async_connect(_socket_, endpoint_iterator,
                    boost::bind(&NodeSession::ConnectAfter, this,
                        boost::asio::placeholders::error));
            LOG(INFO)<<"bind connecct after"<<endl;
        }

        NodeSession::~NodeSession()
        {
            LOG(INFO)<<"+++++++++++++++++++++node session delete"<<endl;
        }

        tcp::socket& NodeSession::GetSocket()
        {
            return _socket_;
        }

        void NodeSession::ConnectAfter(const boost::system::error_code& error)
        {
#ifdef DEBUG
            cout<<"rootsession::start"<<endl;
#endif
            if(!error){
                cout<<"connect after read pb head :"<< _on_recv_msg_->GetInnerMsgHeaderLen()<<endl;
                _on_recv_msg_ = new NodeMessage();
                boost::asio::async_read(_socket_,
                        boost::asio::buffer(_on_recv_msg_->GetInnerMsgHeaderLoc(),
                            _on_recv_msg_->GetInnerMsgHeaderLen()),
                        boost::bind(&NodeSession::HandleProtobufHeaderLen, this,
                            boost::asio::placeholders::error));
            }else
            {
                LOG(ERROR)<<"error happen while connect to ns"<<endl;
                std::cout << boost::system::system_error(error).what() << std::endl;
            }
        }

        void NodeSession::HandleProtobufHeaderLen(const boost::system::error_code& error)
        {
            //cout<<"pb head len : "<< *(_on_recv_msg_->GetInnerMsgHeaderLoc())<<endl;
            LOG(INFO)<<"recv pb len : "<<(_on_recv_msg_->GetInnerMsgLen())<<endl;
            bool alloc = _on_recv_msg_->InnerMsgAlloc();
            if(!error && alloc)
            {
#ifdef DEBUG
                cout<<"rootsession::protobuf head recved"<<endl;
#endif
                boost::asio::async_read(_socket_, boost::asio::buffer(_on_recv_msg_->GetInnerMsgLoc(), 
                            _on_recv_msg_->GetInnerMsgLen()),
                        boost::bind(&NodeSession::HandleProtobufBody, this, 
                            boost::asio::placeholders::error));
            }
            else
            {
                LOG(ERROR)<<"error happen while read head from ns"<<endl;
                std::cout << boost::system::system_error(error).what() << std::endl;
            }
        }

        void NodeSession::HandleProtobufBody( const boost::system::error_code& error)
        {
            bool alloc = _on_recv_msg_->FileAlloc();
            if( !error && alloc)
            {
                LOG(INFO)<<"protobuf body recved";
                boost::asio::async_read(_socket_, boost::asio::buffer(_on_recv_msg_->GetFileBodyLoc(), 
                            _on_recv_msg_->GetFileBodyLen()),
                        boost::bind(&NodeSession::SendbackResult, this, 
                            boost::asio::placeholders::error));
            }
            else
            {
                RAW_LOG(ERROR,"NodeSession::HandleProtobufBody find error");
                std::cout << boost::system::system_error(error).what() << std::endl;
            }
        }

        void NodeSession::HandleFileBody( const boost::system::error_code& error)
        {
            if(!error)
            {
                /*
                   cout<<"Match resved"<<_on_recv_msg_->GetFileBodyLen()<<endl;
                 * todo:error handle
                 * attention;
                 * 
                 boost::asio::async_write(_socket_, 
                 boost::asio::buffer(_on_recv_msg_->write_buf_,_on_recv_msg_->write_len_),  
                 boost::bind(&NodeSession::SendbackResult, this, 
                 boost::asio::placeholders::error));
                 */
            }
            else
            {
                RAW_LOG(ERROR,"NodeSession::HandleFileBody find error");
            }
        }

        void NodeSession::SendbackResult( const boost::system::error_code& error)
        {
            if(!error)
            {
                cout<<"Send back OK!"<<endl;
                SessionService* g_session_service = SessionService::getInstance(); 
                g_session_service->NodeHandler(this, _on_recv_msg_);
                _on_recv_msg_ = new NodeMessage();
                LOG(INFO)<<"Listen data from NS"<<endl;
                boost::asio::async_read(_socket_,
                        boost::asio::buffer(_on_recv_msg_->GetInnerMsgHeaderLoc(),
                            _on_recv_msg_->GetInnerMsgHeaderLen()),
                        boost::bind(&NodeSession::HandleProtobufHeaderLen, this,
                            boost::asio::placeholders::error));
            }
            else
            {
                RAW_LOG(ERROR,"NodeSession::SendbackResult find error");
                std::cout << boost::system::system_error(error).what() << std::endl;
            }
        }

        void NodeSession::Write2ClientOK(const boost::system::error_code& error)
        {
            boost::mutex::scoped_lock locker(_mtx_);
            RAW_LOG(INFO,"Write OK");
            if(!error){
                NodeMessage* msg = _on_send_msg_;
                delete msg;
                _on_send_msg_ = NULL;
            }
            else
            {
                RAW_LOG(ERROR,"SendbackResult 2 client error");
                std::cout << boost::system::system_error(error).what() << std::endl;
            }
        }

        void NodeSession::TrySendMsg(NodeMessage* msg)
        {
            if(msg == NULL)
                return;
            boost::mutex::scoped_lock locker(_mtx_);
            RAW_LOG(INFO,"node try send");
            _on_send_msg_ = msg;
            async_write(_socket_,boost::asio::buffer(
                        _on_send_msg_->GetOutLoc(),
                        _on_send_msg_->GetOutLen()),
                    boost::bind(&NodeSession::Write2ClientOK, this,
                        boost::asio::placeholders::error ));
        }

        string NodeSession::_RandomNum()
        {
            int randnum = 0;
            stringstream rand_num_sstr;
            randnum = rand();
            rand_num_sstr<<randnum;
            return rand_num_sstr.str();
        }

        string NodeSession::_WriteToFile(char* msg, int len)
        {
            string store_name;
            return store_name;
        }
    }
}
