#include "PreCompile.h"
#include "EngineCoreDebug.h"
#include <EnginePlatform/EngineWinDebug.h>
#include "EngineAPICore.h"

namespace UEngineDebug
{
	class DebugTextInfo
	{
	public:
		std::string Text;
		FVector2D Pos;
	};


	// 선언
	std::vector<DebugTextInfo> DebugTexts;


// #ifdef _DEBUG
	FVector2D EngineTextPos = FVector2D::ZERO;
// #endif

#ifdef _DEBUG
	bool IsDebug = true;
#else
	bool IsDebug = false;
#endif

	void SetIsDebug(bool _IsDebug)
	{
		IsDebug = _IsDebug;
	}

	void SwitchIsDebug()
	{
		IsDebug = !IsDebug;
	}

	void CoreOutPutString(std::string_view _Text)
	{
// #ifdef _DEBUG
		// 바로 출력하지 않는다.
		DebugTexts.push_back({ _Text.data(), EngineTextPos});
		EngineTextPos.Y += 20;
// endif 
	}

	void CoreOutPutString(std::string_view _Text, FVector2D _Pos)
	{
// #ifdef _DEBUG
		DebugTexts.push_back({ _Text.data(), _Pos });
// #endif
	}

	void PrintEngineDebugText()
	{
		if (false == IsDebug)
		{
			return;
		}

		// void WinAPIOutPutString(UEngineWinImage * _Image, std::string_view _Text, FVector2D _Pos);
		UEngineWinImage* BackBuffer = UEngineAPICore::GetCore()->GetMainWindow().GetBackBuffer();

		for (size_t i = 0; i < DebugTexts.size(); i++)
		{
			DebugTextInfo& Debug = DebugTexts[i];
			WinAPIOutPutString(BackBuffer, Debug.Text, Debug.Pos);
		}

		EngineTextPos = FVector2D::ZERO;
		DebugTexts.clear();
	}

}
