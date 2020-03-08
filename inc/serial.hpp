#ifndef __SERIAL_HPP
#define __SERIAL_HPP

#include <iostream>
#include <string>

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/timeb.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

// 使用 socat -d -d pty,raw,echo=0 pty,raw,echo=0 创建两个虚拟串口
// 本例中的虚拟串口为
// 		       发送  /dev/pts/3
//           接受  /dev/pts/4
int initSerial();

int sendData(string& data);

// 从串口接受数据 直到遇到 '\0'
// 注意 暂未设置 timeout
int readData(string& data);

#endif