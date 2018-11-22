#include "stdafx.h"
#include "ScanCode.h"

#define SCANCODE_TIMEOUT					(50)

int ScanCode::readCode(MSG *pMsg, std::string &data)
{
	static std::string keydata;
	static DWORD t1, t2;

	// 按下键盘到放开键盘时间间隔大于50ms判断是人为操作键盘
	if (pMsg->message == WM_KEYDOWN) {
		t1 = GetTickCount();
	} else if (pMsg->message == WM_CHAR) {
		keydata += pMsg->wParam;
	} else if (pMsg->message == WM_KEYUP) {
		t2 = GetTickCount();
		if (t2 - t1 > SCANCODE_TIMEOUT) {
			keydata.clear();
		} else {
			if (pMsg->wParam == VK_RETURN && !keydata.empty()) {
				data = keydata;
				keydata.clear();
				return 0;
			}
		}
	} 
	return -1;
}
