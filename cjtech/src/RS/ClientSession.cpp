
#include "ClientSession.h"
#include <iostream>
#include <sys/syscall.h>
#include <pthread.h>
#include <glog/logging.h>
#include <glog/raw_logging.h>

#include "ClientMessage.h"
#include "SessionService.h"

using namespace std;
using boost::asio::ip::tcp;  

extern cjtech::RootServer::SessionService* g_session_service;

namespace cjtech{
    namespace RootServer{
        boost::mutex g_mtx;
        ClientSession::ClientSession(boost::asio::io_service& io_service)
            : _socket_(io_service)
        {

        }

        ClientSession::~ClientSession()
        {
            //        boost::mutex::scoped_lock lock(cout_mtx);
        }

        tcp::socket& ClientSession::socket()
        {
            return _socket_;
        }

        void ClientSession::start()
        {
            std::cout<<"clientsession::start"<<std::endl;
            pthread_t tid =  pthread_self();
            std::cout<<"clientsession::start::tid"<<tid<<std::endl;
            printf("child thread lwpid = %u\n", syscall(SYS_gettid));
            printf("child thread tid = %u\n", pthread_self()); 
            _on_recv_msg_ = new ClientMessage();
            boost::asio::async_read(_socket_, boost::asio::buffer(_on_recv_msg_->GetJsonLenLoa(), 
                        _on_recv_msg_->GetJsonHeaderLen()),
                    boost::bind(&ClientSession::h_json_header_len, this, 
                        boost::asio::placeholders::error));
        }
		

        void ClientSession::h_json_header_len(const boost::system::error_code& error)
        {        
            if(!error)
            {
                pthread_t tid =  pthread_self();
                std::cout<<"clientsession::json_header_len_recved::tid"<<tid<<std::endl;
                std::cout<<"clientsession::json lens recved"<<std::endl;
                _on_recv_msg_->TranJsonLenCharToInt();
                boost::asio::async_read(_socket_, boost::asio::buffer(_on_recv_msg_->GetJsonBodyLoc(), 
                            _on_recv_msg_->GetJsonBodyLen()),
                        boost::bind(&ClientSession::h_json_body, this, 
                            boost::asio::placeholders::error));
            }
            else 
            {
                std::cout<<"---------------------------json header len---"<<std::endl;
            }
        }

        void ClientSession::h_json_body(const boost::system::error_code& error)
        {
            if (!error)
            {
                std::cout<<"ClientSession::json body recved"<<std::endl;
                _on_recv_msg_->ParserJson();
                _on_recv_msg_->TranFileLenToInt();
                if(_on_recv_msg_->GetFileBodyLen() == -1)
                {
                    RecvErrorData();
                    return;
                }
                boost::asio::async_read(_socket_,boost::asio::buffer(_on_recv_msg_->GetFileBodyLoc(),
                            _on_recv_msg_->GetFileBodyLen()),
                        boost::bind(&ClientSession::h_file_body, this, 
                            boost::asio::placeholders::error));
            }
            else
            {
                std::cout<<"---------------------------json header len---"<<std::endl;

            }
        }

        void ClientSession::h_file_body(const boost::system::error_code& error)
        {
            if (!error)
            {	
                /*todo:handle file body
                 *g_ClientSession_manager(_on_recv_msg_);
                 */
                LOG(INFO)<<"ClientSession::file body recved"<<std::endl;
                LOG(INFO)<<_on_recv_msg_->GetJsonString()<<std::endl;
                SessionService* g_session_service = SessionService::getInstance();
                g_session_service->RequsetHandler(this, this->_on_recv_msg_);
                //LOG(INFO) <<"my first info";
                _on_recv_msg_ = NULL;
                //this->start();
            }
            else
            {
                std::cout<<"---------------------------json header len---"<<std::endl;

            }
        }

        void ClientSession::sent_result_back(const boost::system::error_code& error)
        { 
            /*  
            boost::mutex::scoped_lock locker(_mtx_);
            if(!error){
                cout<<"write Ok"<<endl;
                if(_cli_write_buf_.size() == 0)
                {
                    return;
                }
                while( _cli_write_buf_.size()!=0)
                {
                    ClientMessage* msg = _cli_write_buf_.front();
                    if(msg->send_or_not_)
                    {
                        delete msg;
                        _cli_write_buf_.pop_front();
                    }
					else break;
                }
                if( _cli_write_buf_.size()>=1)
                {
                    async_write(_socket_,boost::asio::buffer(
                                _cli_write_buf_.front()->GetOutLoc(),
                                _cli_write_buf_.front()->GetOutLen()),
                            boost::bind(&ClientSession::sent_result_back, this,
                                boost::asio::placeholders::error ));
                    _cli_write_buf_.front()->send_or_not_ = true;
                }
            }else
            {

            }
            */
        }
        void ClientSession::send2cliover_restart(const boost::system::error_code& error)
        {
            if(!error)
            {
                cout<<"+++++++++++++++++++++++start+++again+++++++++++++++++++++++++++++++++"<<endl;
                delete _on_send_msg_; 
                _on_send_msg_ = NULL;
                this->start();
            }
            else
            {
                cout<<"error in restart"<<endl;
                std::cout<<"---------------------------json header len---"<<std::endl;
            }
        }

        void ClientSession::try_send_msg()
        {
            /*这个函数的存在书为了线程的切换*/
            /*boost::mutex::scoped_lock locker(_mtx_);
            cout<<"Do Client Write"<<endl;
            cout<<"Write buf : "<<_cli_write_buf_.front()->GetOutLoc()<<endl;
            cout<<"Write len : "<<_cli_write_buf_.front()->GetOutLen()<<endl;
            int out_que_size = _cli_write_buf_.size();
            if(out_que_size>=1)
            {
                async_write(_socket_,boost::asio::buffer(
                            _cli_write_buf_.front()->GetOutLoc(),
                            _cli_write_buf_.front()->GetOutLen()),
                        boost::bind(&ClientSession::send2cliover_restart, this,
                            boost::asio::placeholders::error ));
                _cli_write_buf_.front()->send_or_not_ = true;
            }*/
            LOG(INFO)<<"Write buf : "<<_on_send_msg_->GetOutLoc()<<endl;
            LOG(INFO)<<"Write len : "<<_on_send_msg_->GetOutLen()<<endl;
                async_write(_socket_,boost::asio::buffer(
                            _on_send_msg_->GetOutLoc(),
                            _on_send_msg_->GetOutLen()),
                        boost::bind(&ClientSession::send2cliover_restart, this,
                            boost::asio::placeholders::error ));
        }

        void ClientSession::add_out_msg(ClientMessage* out_msg)
        {
            //boost::mutex::scoped_lock locker(_mtx_);
            //_cli_write_buf_.push_back(out_msg);
            _on_send_msg_ = out_msg;
            cout<<"out buf : "<<out_msg->GetOutLoc()<<endl;
            cout<<"out len : "<<out_msg->GetOutLen()<<endl;
        }

        void ClientSession::RecvErrorData()
        {
            string error_msg = string("Json Data is Error : ") + _on_recv_msg_->GetJsonString() ;
            _error_data_ = (char*)malloc(error_msg.length());
            memcpy(_error_data_, error_msg.c_str(), error_msg.length());
                async_write(_socket_,boost::asio::buffer(
                            _error_data_,
                            error_msg.length()),
                        boost::bind(&ClientSession::Error_Back2Cli, this,
                            boost::asio::placeholders::error ));
        }
        void ClientSession::Error_Back2Cli(const boost::system::error_code& error)
        {
            free(_error_data_);
        }
    }
}
