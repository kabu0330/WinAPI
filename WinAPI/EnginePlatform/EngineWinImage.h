#pragma once
#include <Windows.h>
#include <EngineBase/EngineMath.h>
#include <EngineBase/Object.h>

// ��� �̹����� (1, 1) �ȼ� �̹�����.

// ���� : ������ �ٽ� Ŭ����(HDC ����)
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
	
	// ���� �����ϴ� DC�� ��¥ ������ ȭ���� �����ִ� MainHDC�̹Ƿ� �״�� ����
	void Create(HDC _DC)
	{
		ImageDC = _DC;
	}

	// �� ��° �����ϴ� DC�� BackBuffer. ���� ������ DC�� ũ�⸦ ���� �ϱ� ���ؼ� �ι�° ���ڷ� ũ�⸦ �޴´�.
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
	HDC ImageDC = nullptr; // �̹��� �� ��ü, ��
	HBITMAP hBitMap = nullptr; // ��ȭ��

	BITMAP Info; // �̹��� ����
};

