#pragma once
#include <Windows.h>
#include <EngineBase/EngineMath.h>

// 모든 이미지는 (1, 1) 픽셀 이미지다.

// 설명 : 렌더링 핵심 클래스(HDC 랩핑)
class UEngineWinImage
{
public:
	// constrcuter destructer
	UEngineWinImage();
	~UEngineWinImage();

	// delete Function
	UEngineWinImage(const UEngineWinImage& _Other) = delete;
	UEngineWinImage(UEngineWinImage&& _Other) noexcept = delete;
	UEngineWinImage& operator=(const UEngineWinImage& _Other) = delete;
	UEngineWinImage& operator=(UEngineWinImage&& _Other) noexcept = delete;

	HDC GetDC()
	{
		return ImageDC;
	}

	void Create(HDC _DC)
	{
		ImageDC = _DC;
	}

	void Create(UEngineWinImage* _TargetImage, FVector2D _Scale);

	void CopyToBit(UEngineWinImage* _TargetImage, const FTransform& _Trans);

protected:

private:
	HDC ImageDC = nullptr; // 이미지 그 자체, 붓
	HBITMAP hBitMap = nullptr; // 도화지

	BITMAP Info; // 이미지 정보
};

