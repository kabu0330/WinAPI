#pragma once
#include <Windows.h>
#include <EngineBase/EngineMath.h>

// ��� �̹����� (1, 1) �ȼ� �̹�����.

// ���� : ������ �ٽ� Ŭ����(HDC ����)
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
	HDC ImageDC = nullptr; // �̹��� �� ��ü, ��
	HBITMAP hBitMap = nullptr; // ��ȭ��

	BITMAP Info; // �̹��� ����
};

