#include "filemonitor.h"
#include "ub.h"

/** get last_modify_time of file */
static int get_mtime(const char *fn, time_t &mt){
	if(NULL == fn){
        return -1;
    }
	struct stat statbuf;
	int ret = stat(fn, &statbuf);
	if (0 == ret){
		mt = statbuf.st_mtime;
		return 0;
	}
	else{
		return -1;
	}
}

/** 判断文件是否更新 */
static bool is_file_update(const char *fname, time_t &last_updatetime){
    time_t mtime;
    int ret = get_mtime(fname, mtime);
    if(ret == 0){
        bool ret = mtime != last_updatetime;
        last_updatetime = mtime;
        return ret;
    }else{
        return false;
    }
}

int FileMonitor::run(callback_func_t callback_fun)
{
	_fm.callback = callback_fun;
	// 启动时立即加载一次
	if(is_file_update(_fm.fname.c_str(), _fm.last_updatetime)){
		_fm.callback(_fm.fname.c_str(),
					 _fm.delay_interval,
					 _fm.data);
	}
	return pthread_create(&_pt, NULL, _callback_wrapper, (void*)&_fm);
}

void * FileMonitor::_callback_wrapper(void *args){
    file_monitor_t *fm = (file_monitor_t*)args;
    const char *fname = fm->fname.c_str();
	uint32_t reload_seconds = fm->scan_interval;
	uint32_t delay_seconds = fm->delay_interval;
	void *data = fm->data;
    time_t last_updatetime = fm->last_updatetime;
	ub_log_initthread(fname);		//打开线程日志
    while(true){
        if(is_file_update(fname, last_updatetime)){
            fm->callback(fname, delay_seconds, data);
        }
        sleep(reload_seconds);
    }
	ub_log_closethread();  //关闭线程日志
}


