#pragma once
#include <Windows.h>
#include <assert.h>


namespace UEngineDebug
{
	// 로우한 레벨의 기능일수록 만드는 사람은 모든 상황에 대응할수 있게 만드므로 
	// 인자가 많고 사용하기가 어렵다.
	void WinAPIOutPutString(UEngineWinImage* _Image, std::string_view _Text, FVector2D _Pos);
}

