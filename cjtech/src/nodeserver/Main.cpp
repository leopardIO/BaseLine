#include<iostream>
#include "RootSession.h"
#include "DBManager.h"
#include "IOServicePool.h"
#include "SessionManager.h"
#include "../../common/includeopencv/interface.h"
#include "NodeUtil.h"
#include <glog/logging.h>   
#include <glog/raw_logging.h>

using namespace std;
using namespace NodeServer;

Matcher* g_pic_matcher;
DBManager* g_db_manager;
IOServicePool* g_io_service_pool = NULL;
SessionManager* g_session_manager; 

int main(int argn, char** argv)
{
    FLAGS_log_dir =  "../log";//the log path is /project_root/log
    FLAGS_stderrthreshold = 0 ;// Copy log messages at or above this level to stderr in addition to logfiles
    google::InitGoogleLogging(argv[0]);
	g_pic_matcher = new Matcher();
    if(argn !=1)
    {
        LOG(WARNING) << "Dot not Need Parameter";
    }
    LOG(INFO) << "The Log Path is project_root/log";
    if((access (INDEXPATH,F_OK)) == 0)
    {
        g_pic_matcher->loadIndex(INDEXPATH);
    }
    else
    {
        g_pic_matcher->train(TRANDIR, FEATUREPATH, INDEXPATH);
    }
	
	g_io_service_pool = new IOServicePool(1);//传入的线程池的大小
    if(g_io_service_pool == NULL)
    {
        LOG(FATAL) << "Main::New IOServicePool ERR";
    }
	g_session_manager = new SessionManager();
    if(g_session_manager == NULL)
    {
        LOG(FATAL) << "Main::New SessionManager ERR";
    }
    RootSession *root_session = g_session_manager->CreateSession<RootSession>(6002);
    if(root_session == NULL)
    {
        LOG(FATAL) << "Main::New RootSession ERR";
    }
    root_session->Run();
	g_io_service_pool->start();
    g_db_manager = new DBManager();
    if(g_db_manager == NULL)
    {
        LOG(FATAL) << "Main::New DBManager ERR";
    }
    g_db_manager->init();
	g_io_service_pool->join();
    /*while(true)
    {
        sleep(1000);
    }*/
    return 0;
}
