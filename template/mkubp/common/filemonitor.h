/**
  * @file: filemonitor.h
  * @brief:
  *     文件监控框架类
  * 使用示例
  *     //定义自己的文件更新回调函数
  *     int callback_fun(const char *fname,
  *                      uint32_t delay_seconds,
  *                      void *data)
  *     {...}
  *
  *     //创建对象, 监控 ./conf/file.conf 文件, 每分钟检查一次
  *     FileMonitor fm();
  *     //初始化，监控 ./conf/file.conf 文件, 每分钟检查一次
  *     //reload之后的延迟时间
  *     fm.init("./conf/file.conf", 60, 1, NULL);
  *     //启动监控线程
  *     fm.run(callback_fun);
  *     //等待线程结束
  *     fm.join();
  *
  * @author: mawentao (mawentao@baidu.com)
  * @date: 2012-07-01
  */

#ifndef __FILEMONITOR_H__
#define __FILEMONITOR_H__

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
  * @brief 文件监控框架类
  */
class FileMonitor{
public:
    //! 文件发生更新后的回调处理函数
    typedef int (*callback_func_t)(const char *fname, uint32_t delay_seconds, void *data);

    struct file_monitor_t{
		std::string fname;       //!< 监控的文件
		uint32_t scan_interval;  //!< 两次扫描的时间间隔（单位:s）
		uint32_t delay_interval; //!< 文件reload后的延迟时间（单位:s）
		void *data;				 //!< 用户数据
        callback_func_t callback;//!< 用户回调函数
		time_t last_updatetime;  //!< 文件上次更新时间
    };

    /**
      * @brief: 构造函数
      * @param: fname : const char* 被监控的文件名
      * @param: scan_interval : int 监控时间时间间隔（单位：s）
      */
    FileMonitor(){}

    /**
	  * @biref 初始化文件监控类
	  * @param fname : const char* 被监控的文件名
      * @param reload_seconds : uint32_t 监控时间时间间隔（单位：s）
	  * @param delay_seconds : uint32_t 文件reload后的延迟时间（单位:s）
	  * @param data : void* 用户数据
	  * @return void
      */
    void init(const char *fname, uint32_t reload_seconds, uint32_t delay_seconds, void *data){
        _fm.fname = fname;
		_fm.scan_interval = reload_seconds;
		_fm.delay_interval = delay_seconds;
		_fm.data = data;
		_fm.last_updatetime = 0;
    }

    /**
      * @brief: 启动文件监控线程
      * @param: 文件更新后的用户回调函数
      * @return: int (同pthread_create的返回值)
      *          0 : 成功
      *          其他 : 错误码
      */
    int run(callback_func_t callback_fun);

    /**
      * @brief: 等待线程结束
      * @return: int (同pthread_join监控线程)
      */
    int join(){
        return pthread_join(_pt, NULL);
    }

private:
    file_monitor_t _fm;
    pthread_t _pt;            //!< 监控线程

    static void *_callback_wrapper(void *);  //! 监控线程使用的回调函数
};


#endif

