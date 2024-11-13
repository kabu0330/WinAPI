#pragma once
#include <Windows.h>
#include <EngineBase/EngineMath.h>
#include <EngineBase/Object.h>

// 모든 이미지는 (1, 1) 픽셀 이미지다.

// 설명 : 렌더링 핵심 클래스(HDC 랩핑)
class UEngineWinImage : public UObject
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
	
	// 최초 생성하는 DC는 진짜 윈도우 화면을 보여주는 MainHDC이므로 그대로 리턴
	void Create(HDC _DC)
	{
		ImageDC = _DC;
	}

	// 두 번째 생성하는 DC는 BackBuffer. 최초 윈도우 DC와 크기를 같게 하기 위해서 두번째 인자로 크기를 받는다.
	void Create(UEngineWinImage* _TargetImage, FVector2D _Scale);

	void CopyToBit(UEngineWinImage* _TargetImage, const FTransform& _Trans);

	void CopyToTrans(UEngineWinImage* _TargetImage, const FTransform& _RenderTrans, const FTransform& _LTImageTrans, UColor _Color = UColor(255, 0, 255, 0));

	void CopyToAlpha(UEngineWinImage* _TargetImage, const FTransform& _RenderTrans, const FTransform& _LTImageTrans, unsigned char _Alpha);

	void Load(UEngineWinImage* _TargetImage, std::string_view _Path);

	FVector2D GetImageScale() const
	{
		return { Info.bmWidth, Info.bmHeight };
	}

	UColor GetColor(FVector2D _Point, UColor _DefaultColor = UColor::WHITE)
	{
		return GetColor(_Point.ConvertToPoint(), _DefaultColor);
	}

	UColor GetColor(FIntPoint _Point, UColor _DefaultColor);

protected:

private:
	HDC ImageDC = nullptr; // 이미지 그 자체, 붓
	HBITMAP hBitMap = nullptr; // 도화지

	BITMAP Info; // 이미지 정보
};

