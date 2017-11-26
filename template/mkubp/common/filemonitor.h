/**
  * @file: filemonitor.h
  * @brief:
  *     �ļ���ؿ����
  * ʹ��ʾ��
  *     //�����Լ����ļ����»ص�����
  *     int callback_fun(const char *fname,
  *                      uint32_t delay_seconds,
  *                      void *data)
  *     {...}
  *
  *     //��������, ��� ./conf/file.conf �ļ�, ÿ���Ӽ��һ��
  *     FileMonitor fm();
  *     //��ʼ������� ./conf/file.conf �ļ�, ÿ���Ӽ��һ��
  *     //reload֮����ӳ�ʱ��
  *     fm.init("./conf/file.conf", 60, 1, NULL);
  *     //��������߳�
  *     fm.run(callback_fun);
  *     //�ȴ��߳̽���
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
  * @brief �ļ���ؿ����
  */
class FileMonitor{
public:
    //! �ļ��������º�Ļص�������
    typedef int (*callback_func_t)(const char *fname, uint32_t delay_seconds, void *data);

    struct file_monitor_t{
		std::string fname;       //!< ��ص��ļ�
		uint32_t scan_interval;  //!< ����ɨ���ʱ��������λ:s��
		uint32_t delay_interval; //!< �ļ�reload����ӳ�ʱ�䣨��λ:s��
		void *data;				 //!< �û�����
        callback_func_t callback;//!< �û��ص�����
		time_t last_updatetime;  //!< �ļ��ϴθ���ʱ��
    };

    /**
      * @brief: ���캯��
      * @param: fname : const char* ����ص��ļ���
      * @param: scan_interval : int ���ʱ��ʱ��������λ��s��
      */
    FileMonitor(){}

    /**
	  * @biref ��ʼ���ļ������
	  * @param fname : const char* ����ص��ļ���
      * @param reload_seconds : uint32_t ���ʱ��ʱ��������λ��s��
	  * @param delay_seconds : uint32_t �ļ�reload����ӳ�ʱ�䣨��λ:s��
	  * @param data : void* �û�����
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
      * @brief: �����ļ�����߳�
      * @param: �ļ����º���û��ص�����
      * @return: int (ͬpthread_create�ķ���ֵ)
      *          0 : �ɹ�
      *          ���� : ������
      */
    int run(callback_func_t callback_fun);

    /**
      * @brief: �ȴ��߳̽���
      * @return: int (ͬpthread_join����߳�)
      */
    int join(){
        return pthread_join(_pt, NULL);
    }

private:
    file_monitor_t _fm;
    pthread_t _pt;            //!< ����߳�

    static void *_callback_wrapper(void *);  //! ����߳�ʹ�õĻص�����
};


#endif

