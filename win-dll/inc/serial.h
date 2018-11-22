#pragma once

#include <string>
#include "baseDll.h"

// template class DLL_API basic_string<char, char_traits<char>, allocator<char>>;

class DLL_API Serial
{
public:
	Serial(DWORD baudRate = 57600, BYTE  byteSize = 8, BYTE  parityBit = NOPARITY, BYTE  stopBit = ONESTOPBIT);
	Serial(std::string portNum, DWORD baudRate = 57600, BYTE  byteSize = 8, BYTE  parityBit = NOPARITY, BYTE  stopBit = ONESTOPBIT);
	~Serial();

private:
	HANDLE m_hComm;		// 通信设备
	std::string m_portNum;	// 串口号
	std::string m_portCode;	// 串口编码
	DWORD m_dwBaudRate; // 波特率
	BYTE  m_byteSize;	// 数据位
	BYTE  m_parityBit;  // 校验位
	BYTE  m_stopBit;	// 停止位
	bool  m_bOpen;		// 串口开关标志

private:
	enum BufferSize {
		MIN_BUFFER_SIZE = 256,
		BUFFER_SIZE = 512,
		MAX_BUFFER_SIZE = 1024
	};

public:
	bool open();																// 打开串口
	void close();																// 关闭串口
	int write(char *data);														// 发送数据
	int write(unsigned char *data, DWORD dwLegnth);								// 发送数据
	int write(void *data, DWORD dwLegnth);										// 发送数据
	int read(char *buffer, DWORD dwLength, DWORD timeout = 0);					// 读取数据
	int read(unsigned char *buffer, DWORD dwLength, DWORD timeout = 0);			// 读取数据
	int read(void *buffer, DWORD dwLength);										// 读取数据

	// 设置串口号
	void setPortNum(const std::string &portNum) { this->m_portNum = portNum; }
	// 设置串口编码
	void setPortCode(const std::string &portCode) { this->m_portCode = portCode; }
	// 设置波特率
	void setBaudRate(const unsigned long baudRate) { this->m_dwBaudRate = baudRate; }
	// 设置数据位
	void setByteSize(const unsigned char byteSize) { this->m_byteSize = byteSize; }
	// 设置检验位
	void setParityBit(const unsigned char parityBit) { this->m_parityBit = parityBit; }
	// 设置停止位
	void setStopBit(const unsigned char stopBit) { this->m_stopBit = stopBit; }

	// 获取串口号
	std::string getPortNum() { return m_portNum; }
	// 获取串口号
	std::string getPortCode() { return m_portCode; }
	// 获取波特率
	unsigned long getBaudRate() { return m_dwBaudRate; }
	// 获取数据位
	unsigned char getByteSize() { return m_byteSize; }
	// 获取检验位
	unsigned char getParityBit() { return m_parityBit; }
	// 获取停止位
	unsigned char getStopBit() { return m_stopBit; }
	// 获取连接状态
	bool getOpen() { return m_bOpen; }
};