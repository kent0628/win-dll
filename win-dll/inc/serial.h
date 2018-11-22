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
	HANDLE m_hComm;		// ͨ���豸
	std::string m_portNum;	// ���ں�
	std::string m_portCode;	// ���ڱ���
	DWORD m_dwBaudRate; // ������
	BYTE  m_byteSize;	// ����λ
	BYTE  m_parityBit;  // У��λ
	BYTE  m_stopBit;	// ֹͣλ
	bool  m_bOpen;		// ���ڿ��ر�־

private:
	enum BufferSize {
		MIN_BUFFER_SIZE = 256,
		BUFFER_SIZE = 512,
		MAX_BUFFER_SIZE = 1024
	};

public:
	bool open();																// �򿪴���
	void close();																// �رմ���
	int write(char *data);														// ��������
	int write(unsigned char *data, DWORD dwLegnth);								// ��������
	int write(void *data, DWORD dwLegnth);										// ��������
	int read(char *buffer, DWORD dwLength, DWORD timeout = 0);					// ��ȡ����
	int read(unsigned char *buffer, DWORD dwLength, DWORD timeout = 0);			// ��ȡ����
	int read(void *buffer, DWORD dwLength);										// ��ȡ����

	// ���ô��ں�
	void setPortNum(const std::string &portNum) { this->m_portNum = portNum; }
	// ���ô��ڱ���
	void setPortCode(const std::string &portCode) { this->m_portCode = portCode; }
	// ���ò�����
	void setBaudRate(const unsigned long baudRate) { this->m_dwBaudRate = baudRate; }
	// ��������λ
	void setByteSize(const unsigned char byteSize) { this->m_byteSize = byteSize; }
	// ���ü���λ
	void setParityBit(const unsigned char parityBit) { this->m_parityBit = parityBit; }
	// ����ֹͣλ
	void setStopBit(const unsigned char stopBit) { this->m_stopBit = stopBit; }

	// ��ȡ���ں�
	std::string getPortNum() { return m_portNum; }
	// ��ȡ���ں�
	std::string getPortCode() { return m_portCode; }
	// ��ȡ������
	unsigned long getBaudRate() { return m_dwBaudRate; }
	// ��ȡ����λ
	unsigned char getByteSize() { return m_byteSize; }
	// ��ȡ����λ
	unsigned char getParityBit() { return m_parityBit; }
	// ��ȡֹͣλ
	unsigned char getStopBit() { return m_stopBit; }
	// ��ȡ����״̬
	bool getOpen() { return m_bOpen; }
};