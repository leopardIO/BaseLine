/*************************************************************************
	> File Name: HeadStructMessage.h
	> Author: zhonghx
	> Mail: zhonghongxia@foxmail.com
	> Created Time: Sat 19 Aug 2015 11:43:37 AM CST	
 ************************************************************************/

#include "SearchSession.h"
#include<iostream>
#include "NodeUtil.h"
#include "SessionManager.h"
#include "ClientSession.h"
#include "DBManager.h"
#include <glog/logging.h>
#include <glog/raw_logging.h> 

using namespace std;

extern NodeServer::SessionManager *g_session_manager;
extern NodeServer::IOServicePool* g_io_service_pool;
extern NodeServer::SessionManager* g_session_manager; 
extern NodeServer::DBManager * g_db_manager;
namespace NodeServer
{
    
    SearchSession::SearchSession(SearchProtoMessage proto_msg , string file_path, uint32_t id)
        :_proto_msg_(proto_msg) , _file_path_(file_path) , _client_session_id_(id)
    {

    }
	SearchSession::~SearchSession()
	{

	}
	void SearchSession::Start()
	{
			/*
			 * todo:error handle
			 * attention;
			 * */
            //得到测试结果
			string filepath =  _file_path_;
			string result = g_pic_matcher->match( filepath.c_str(), FEATUREPATH, TRANDIR);
            LOG(INFO)<<"SearchSession :: Match Result : "<<result;
            int index = result.find("&&", 0);
            string match_name = result.substr( 0, index );
            
            cout <<"fuch match name "<<match_name<<endl;

            int index_pit = result.find(".",0);
            string only_name = result.substr( 0, index_pit );
            string tracker = "/mj/tracker/" + only_name + ".zip";

            char* name_c = (char*)match_name.c_str();
            vector<int> match_pic_id = g_db_manager->GetPicID(name_c , match_name.length());

            string mj_id = g_db_manager->GetMJID(match_pic_id);
			string response = g_db_manager->Query(_file_name_.c_str());
			SearchResultMessage  search_result;
            search_result.set_picture_name(_file_name_);
            search_result.set_result_length(response.length());
            search_result.set_task_id(_proto_msg_.task_id());
            search_result.set_mj_id(mj_id);
            search_result.set_trackerurl(tracker);
            if(match_name.length() == 0 )
            {
                search_result.set_status(0);//失败是0
                search_result.set_mj_id("");
                search_result.set_trackerurl("");

            }
            else
            {
                if(mj_id.length() == 0 || tracker.length() == 0)
                {
                    search_result.set_status(0);//失败是0
                    search_result.set_mj_id("");
                    search_result.set_trackerurl("");
                    cout<<mj_id.length() << "        "<<tracker.length()<<endl;;
                }
                else
                search_result.set_status(1); //成功是1
            }
            string proto_str_tmp ;
            search_result.SerializeToString(&proto_str_tmp);
            cout<<"search_result total length : "<< proto_str_tmp.length() <<endl;
            string response_msg =
               Session::FormMessage(SEARCH_REQUEST_RESULT_FROM_NODE,proto_str_tmp,response); 
            ClientSession *client_session = dynamic_cast<ClientSession *> (g_session_manager->Get(_client_session_id_));
            client_session->SetWritePacket(response_msg);
            LOG(INFO) <<"SearchSession :: copied result to ClientSession";
            g_session_manager->Recycle(GetSessionID());
	}
}

