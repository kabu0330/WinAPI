#include "PreCompile.h"
#include "EngineSprite.h"
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineWinImage.h>

UEngineSprite::UEngineSprite()
{
}

UEngineSprite::~UEngineSprite()
{
}

void UEngineSprite::PushData(UEngineWinImage* _Image, const FTransform& _Trans)
{
	// 원본 이미지 크기가 0이 아니면 된다.
	if (true == _Trans.Scale.IsZero())
	{
		MSGASSERT("크기가 0인 이미지를 만들수 없습니다");
	}

	// 리스트 이니셜라이저
	Data.push_back({ _Image, _Trans });
}