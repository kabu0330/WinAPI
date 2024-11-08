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
	// ���� �̹��� ũ�Ⱑ 0�� �ƴϸ� �ȴ�.
	if (true == _Trans.Scale.IsZero())
	{
		MSGASSERT("ũ�Ⱑ 0�� �̹����� ����� �����ϴ�");
	}

	// ����Ʈ �̴ϼȶ�����
	Data.push_back({ _Image, _Trans });
}