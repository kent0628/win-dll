#include "stdafx.h"
#include "serial.h"
#include "strUtil.h"

Serial::Serial(DWORD baudRate, BYTE byteSize, BYTE parityBit, BYTE stopBit)
	: m_dwBaudRate(baudRate), m_byteSize(byteSize), m_parityBit(parityBit), m_stopBit(stopBit), m_bOpen(false)
{
}

Serial::Serial(std::string portNum, DWORD baudRate, BYTE byteSize, BYTE parityBit, BYTE stopBit)
	: m_portNum(portNum), m_dwBaudRate(baudRate), m_byteSize(byteSize), m_parityBit(parityBit), m_stopBit(stopBit), m_bOpen(false)
{
}

Serial::~Serial()
{
}

// 打开串口成功,返回 true
bool Serial::open()
{
	std::string tmp;
	wchar_t *p;

	tmp.append("\\\\.\\").append(m_portNum);
	p = StrUtil::str2wchar(tmp);
	//允许读和写, 独占方式, 打开而不是创建, 同步方式
	m_hComm = CreateFile(p, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	StrUtil::freeWchar(p);
	if (m_hComm == INVALID_HANDLE_VALUE) {
		return false;
	}

	SetupComm(m_hComm, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE); // 设置读写缓冲区大小

	COMMTIMEOUTS TimeOuts;
	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;
	TimeOuts.WriteTotalTimeoutMultiplier = 100;
	TimeOuts.WriteTotalTimeoutConstant = 500;
	SetCommTimeouts(m_hComm, &TimeOuts); //设置超时 

	DCB dcb;
	GetCommState(m_hComm, &dcb);
	dcb.BaudRate = m_dwBaudRate;
	dcb.ByteSize = m_byteSize;
	dcb.Parity = m_parityBit;
	dcb.StopBits = m_stopBit;
	dcb.fDtrControl = DTR_CONTROL_ENABLE;

	if (!SetCommState(m_hComm, &dcb)) {
		CloseHandle(m_hComm);
		return false;
	}

	if (EscapeCommFunction(m_hComm, SETDTR) == 0) {
		CloseHandle(m_hComm);
		return false;
	}

	//在读写串口前，用 PurgeComm 函数清空缓冲区
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	m_bOpen = true;
	return true;
}

// 关闭串口
void Serial::close()
{
	if (m_bOpen) {
		// 禁止串行端口所有事件
		SetCommMask(m_hComm, 0);
		// 清除数据终端就绪信号
		EscapeCommFunction(m_hComm, CLRDTR);
		// 丢弃通信资源的输出或输入缓冲区字符并终止在通信资源上挂起的读、写操操作  
		PurgeComm(m_hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		// 关闭串口
		CloseHandle(m_hComm);
		m_bOpen = false;
	}
}

int Serial::write(char *data)
{
	return write((void *)data, strlen(data));
}

int Serial::write(unsigned char *data, DWORD dwLegnth)
{
	return write((void *)data, dwLegnth);
}

// 向串口发送数据
int Serial::write(void *data, DWORD dwLength)
{
	DWORD dwTx = 0;
	BOOL ret = FALSE;

	if (!m_bOpen) {
		return -1;
	}

	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	ret = WriteFile(m_hComm, data, dwLength, &dwTx, NULL);
	if (ret == FALSE) {
		return -1;
	} else {
		return (int)dwTx;
	}
}

int Serial::read(char *buffer, DWORD dwLength, DWORD timeout)
{
	int ret;

	ret = read((unsigned char *)buffer, dwLength - 1, timeout);
	if (ret >= 0) {
		buffer[ret] = '\0';
	}
	return ret;
}

int Serial::read(unsigned char *buffer, DWORD dwLength, DWORD timeout)
{
	int ret = 0;
	DWORD len = 0;
	DWORD t1, t2;

	t2 = t1 = GetTickCount();
	while (t2 - t1 <= timeout) {
		ret = read((void *)(buffer + len), dwLength - len);
		if (ret != -1) {
			len += ret;
			ret = 0;
			if (len >= dwLength) {
				break;
			}
		}
		t2 = GetTickCount();
	}
	return len;
}

// 从串口中读取数据
int Serial::read(void *buffer, DWORD dwLength)
{
	if (!m_bOpen) {
		return -1;
	}

	COMSTAT comStat;
	DWORD dwError = 0;
	if (ClearCommError(m_hComm, &dwError, &comStat) && dwError > 0) {
		PurgeComm(m_hComm, PURGE_RXABORT | PURGE_RXCLEAR);
	}

	DWORD dwRx = 0; // 读取的字节数
	BOOL ret = FALSE;
	ret = ReadFile(m_hComm, buffer, dwLength, &dwRx, NULL);	// 读取数据
	if (ret == FALSE) {
		return -1;
	} else {
		return (int)dwRx;
	}
}
