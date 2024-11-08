#pragma once
#include <EngineBase/Object.h>


// 설명 :
class UEngineSprite : public UObject
{
public:
	class USpriteData
	{
	public:
		// 이 이미지의
		class UEngineWinImage* Image;
		// 여기서부터 xx 크기까지
		FTransform Transform;
		// 잘라서 쓰겠다.
	};

	// constrcuter destructer
	UEngineSprite();
	~UEngineSprite();

	// delete Function
	UEngineSprite(const UEngineSprite& _Other) = delete;
	UEngineSprite(UEngineSprite&& _Other) noexcept = delete;
	UEngineSprite& operator=(const UEngineSprite& _Other) = delete;
	UEngineSprite& operator=(UEngineSprite&& _Other) noexcept = delete;

	void PushData(class UEngineWinImage* Image, const FTransform& _Trans);

	USpriteData GetSpriteData(int _Index = 0)
	{
		if (_Index >= Data.size())
		{
			MSGASSERT("스프라이트의 인덱스를 오버하여 사용하려고 했습니다." + GetName());
		}

		return Data[_Index];
	}

	void ClearSpriteData()
	{
		Data.clear();
	}

protected:

	std::vector<USpriteData> Data;
};

