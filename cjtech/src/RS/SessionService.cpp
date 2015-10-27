#include "SessionService.h"

#include<iostream>

#include "ClientSession.h"
#include "NodeSession.h"
#include "ClientMessage.h"
#include "NodeMessage.h"

using namespace std;
using namespace cjtech::RootServer;

SessionService* SessionService::_pSessionService_ = NULL;
namespace cjtech
{
    namespace RootServer
    {
        SessionService* SessionService::getInstance()
        {
            if(_pSessionService_ == NULL)
            {
                _pSessionService_ = new SessionService();
            }
            return _pSessionService_;
        }

        SessionService::SessionService()
        {
            node_manager_ = new NodeManager(1);
            _task_id_ = 0;
        }
        
        SessionService::~SessionService()
        {

        }

        void SessionService::RequsetHandler( ClientSession* cli_session,
                ClientMessage* req_msg)
        {
            /*1找到为其服务的NodeSession，2将消息从json转到对应的protobuf.
             *3注意满负载时，需要直接回绝客户端*/
            int taskid = _task_id_;
            NodeMessage* node_msg = new NodeMessage();
            _data_switcher_.Json2PB( req_msg, node_msg);
            node_msg->SetTaskID(taskid);
            node_msg->SetBufMsg2Node();
            NodeSession* temp_node_session = node_manager_->GetNodeByRotate();
            temp_node_session->TrySendMsg(node_msg);
            _mtx_.lock();
            _taskid_clises_[taskid] = cli_session;
            _taskid_ndeses_[taskid] = temp_node_session;
			_task_id_++;
            _mtx_.unlock();
        }

        void SessionService::NodeHandler( NodeSession* node_session,
                NodeMessage* req_msg)
        {
            int task_id = req_msg->inner_msg.task_id();
            _mtx_.lock();
            ClientSession* cli_session = _taskid_clises_[task_id];
            _mtx_.unlock();
            ClientMessage* cli_msg = new ClientMessage();
            _data_switcher_.PB2Json( req_msg, cli_msg);
            //cout<<"cli msg :"<<cli_msg->GetOutLoc()<<endl;;
            //cout<<"cli len :"<<cli_msg->GetOutLen()<<endl;;
            cli_session->add_out_msg( cli_msg);
            cli_session->try_send_msg();
        }
        
        void SessionService::Run()
        {
            node_manager_->run();
        }
    }
}
