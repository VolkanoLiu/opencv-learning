#include "../inc/serial.hpp"

using namespace std;

int fd_0, fd_1; /* File descriptor for the port */
char uart_0[] = "/dev/pts/3";
char uart_1[] = "/dev/pts/4";

// 使用 socat -d -d pty,raw,echo=0 pty,raw,echo=0 创建两个虚拟串口
// 本例中的虚拟串口为
// 		       发送  /dev/pts/3
//           接受  /dev/pts/4
int initSerial() {
  fd_0 = open(uart_0, O_RDWR | O_NOCTTY | O_NDELAY);
  fd_1 = open(uart_1, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd_0 == -1) {
    cout << "Error: cannot connect to the port "
         << "\'" << uart_0 << "\'" << endl;
    return 0;
  }
  if (fd_1 == -1) {
    cout << "Error: cannot connect to the port "
         << "\'" << uart_0 << "\'" << endl;
    return 0;
  }
  fcntl(fd_0, F_SETFL, 0);
  fcntl(fd_1, F_SETFL, FNDELAY);
  return 1;
}

int sendData(string& data) {
  const char* str = data.c_str();
  long int n = write(fd_0, str, strlen(str) + 1);
  if (n < 0) {
    return 0;
  } else {
    return 1;
  }
}

// 从串口接受数据 直到遇到 '\0'
// 注意 暂未设置 timeout
int readData(string& data) {
  char c;
  long int n;
  data = "";
  do {
    n = read(fd_1, &c, 1);
    if (n > -1) {
      data += c;
    }
  } while (c != '\0');
  return 0;
}
