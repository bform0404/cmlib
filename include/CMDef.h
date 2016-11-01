#ifndef __CM_DEF_H__
#define __CM_DEF_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <stdint.h>
#include <map>
#include <string.h>
#include <memory>
//每个链接的协议缓存大小
#define MAX_RECV_BUFFER_LEN 40960
//epoll的最大连接数
#define MAX_EPOLL_FDZISE  10000
//每次接收的最大包体
#define MAX_RECV_LEN 10240

#endif
