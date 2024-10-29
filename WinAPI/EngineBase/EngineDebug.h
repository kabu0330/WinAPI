#pragma once
#include <Windows.h>
#include <assert.h>

#define MSGASSERT(VALUE) std::string ErrorText = VALUE; MessageBoxA(nullptr, ErrorText.c_str(), "ġ���� ����", MB_OK); assert(false);

// std::string ErrorText = "���� �̸��� ������ Ŭ������ 2�� ����߽��ϴ�" + std::string(_Class.lpszClassName);
// std::string ���ο� ��� �ִ� �ɹ����� => std::string => std::vector<char>

// std::vector<char> char* = new char[100];
// ErrorText const char* �������ִ� �Լ��� c_str()
// const char* Text = ErrorText.c_str();

namespace UEngineDebug
{
	// Visual Studio ��� â�� ���ڸ� ������ִ� �Լ�
	void OutPutString(const std::string& Text);
}