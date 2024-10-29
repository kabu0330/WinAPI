#pragma once
#include <Windows.h>
#include <assert.h>

#define MSGASSERT(VALUE) std::string ErrorText = VALUE; MessageBoxA(nullptr, ErrorText.c_str(), "치명적 에러", MB_OK); assert(false);

// std::string ErrorText = "같은 이름의 윈도우 클래스를 2번 등록했습니다" + std::string(_Class.lpszClassName);
// std::string 내부에 들고 있는 맴버변수 => std::string => std::vector<char>

// std::vector<char> char* = new char[100];
// ErrorText const char* 리턴해주는 함수가 c_str()
// const char* Text = ErrorText.c_str();

namespace UEngineDebug
{
	// Visual Studio 출력 창에 문자를 출력해주는 함수
	void OutPutString(const std::string& Text);
}