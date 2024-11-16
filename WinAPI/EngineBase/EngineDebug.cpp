#include "PreCompile.h"
#include "EngineDebug.h"

namespace UEngineDebug
{
	void OutPutString(const std::string& Text)
	{
		std::string ResultText = Text + "\n";
		OutputDebugStringA(ResultText.c_str());
	}

    // 포맷팅된 문자열 출력 함수
    void OutPutFormattedString(const char* Format, ...)
    {
        char Buffer[512]; // 출력 버퍼
        va_list Args;     // 가변 인자 목록

        va_start(Args, Format);                // 가변 인자 시작
        vsnprintf_s(Buffer, sizeof(Buffer), _TRUNCATE, Format, Args); // 포맷팅
        va_end(Args);                          // 가변 인자 종료

        OutPutString(Buffer); // 최종 문자열 출력
    }

}