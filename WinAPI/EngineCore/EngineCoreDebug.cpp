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
	FVector2D EngineTextBotPos = FVector2D::ZERO;
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
		DebugTexts.push_back({ _Text.data(), EngineTextPos });
		EngineTextPos.Y += 20;
		// endif 
	}

	void CoreOutPutString(std::string_view _Text, FVector2D _Pos)
	{
// #ifdef _DEBUG
		DebugTexts.push_back({ _Text.data(), _Pos });
// #endif
	}

	class DebugRenderInfo
	{
	public:
		FTransform Trans;
		EDebugPosType Type;
	};


	std::vector<DebugRenderInfo> DebugPoses;

	void CoreDebugRender(FTransform _Trans, EDebugPosType _Type)
	{
		DebugPoses.push_back({ _Trans, _Type });
	}

	void PrintEngineDebugRender()
	{
		if (false == IsDebug)
		{
			return;
		}

		// void WinAPIOutPutString(UEngineWinImage * _Image, std::string_view _Text, FVector2D _Pos);
		UEngineWinImage* BackBuffer = UEngineAPICore::GetCore()->GetMainWindow().GetBackBuffer();
		HBRUSH hBrushEllipse = CreateSolidBrush(RGB(0, 0 ,255));

		for (size_t i = 0; i < DebugTexts.size(); i++)
		{
			DebugTextInfo& Debug = DebugTexts[i];
			WinAPIOutPutString(BackBuffer, Debug.Text, Debug.Pos);
		}

		EngineTextPos = FVector2D::ZERO;
		DebugTexts.clear();

		FTransform Trans;
		Trans.Scale = FVector2D(6, 6);


		for (size_t i = 0; i < DebugPoses.size(); i++)
		{

			EDebugPosType Type = DebugPoses[i].Type;

			FVector2D LT = DebugPoses[i].Trans.CenterLeftTop();
			FVector2D RB = DebugPoses[i].Trans.CenterRightBottom();

			switch (Type)
			{
			case UEngineDebug::EDebugPosType::Rect:
				Rectangle(BackBuffer->GetDC(), LT.iX(), LT.iY(), RB.iX(), RB.iY());
				break;
			case UEngineDebug::EDebugPosType::Circle:
				SelectObject(BackBuffer->GetDC(), hBrushEllipse);
				Ellipse(BackBuffer->GetDC(), LT.iX(), LT.iY(), RB.iX(), RB.iY());
				break;
			default:
				break;
			}
		}

		DebugPoses.clear();
		DeleteObject(hBrushEllipse);
	}

	void CoreDebugBox(FTransform _Trans)
	{

	}

}
