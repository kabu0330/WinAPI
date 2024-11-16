#include "PreCompile.h"
#include "EngineDebug.h"

namespace UEngineDebug
{
	void OutPutString(const std::string& Text)
	{
		std::string ResultText = Text + "\n";
		OutputDebugStringA(ResultText.c_str());
	}

    // �����õ� ���ڿ� ��� �Լ�
    void OutPutFormattedString(const char* Format, ...)
    {
        char Buffer[512]; // ��� ����
        va_list Args;     // ���� ���� ���

        va_start(Args, Format);                // ���� ���� ����
        vsnprintf_s(Buffer, sizeof(Buffer), _TRUNCATE, Format, Args); // ������
        va_end(Args);                          // ���� ���� ����

        OutPutString(Buffer); // ���� ���ڿ� ���
    }

}