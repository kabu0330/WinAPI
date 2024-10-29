#include "PreCompile.h"
#include "EngineWinImage.h"
#include <EngineBase/EngineDebug.h>

UEngineWinImage::UEngineWinImage()
{
}

UEngineWinImage::~UEngineWinImage()
{
}

void UEngineWinImage::Create(UEngineWinImage* _TargetImage, FVector2D _Scale)
{
	if (nullptr == _TargetImage)
	{
		MSGASSERT("Main WindowDC�� �Ű������� ���� �ʰ� �̹����� ������ �� �����ϴ�.");
		return;
	}

	HBITMAP NewBitMap = static_cast<HBITMAP>(CreateCompatibleBitmap(_TargetImage->GetDC(), _Scale.iX(), _Scale.iY()));
	HDC NewImageDC = CreateCompatibleDC(_TargetImage->GetDC());

	// HDC�� �������ص� (1, 1) �̹����� ����Ǿ� �־�, (1, 1) �̹����� ���� �޸� �Ҵ��� �������ش�.
	// �̷ν� ���� ������ NewImageDC�� ����� �̹����� ����(nullptr) ���°� �ȴ�?
	HBITMAP OldBitMap = static_cast<HBITMAP>(SelectObject(NewImageDC, NewBitMap));
	DeleteObject(OldBitMap);

	hBitMap = NewBitMap;
	ImageDC = NewImageDC;

	GetObject(hBitMap, sizeof(BITMAP), &Info);
}

void UEngineWinImage::CopyToBit(UEngineWinImage* _TargetImage, const FTransform& _Trans)
{
	if (nullptr == _TargetImage)
	{
		MSGASSERT("������� ���(_TargetImage)�� �������� �ʽ��ϴ�.");
		return;
	}

	HDC CopyDC = ImageDC;
	HDC TargetDC = _TargetImage->ImageDC;

	FVector2D LeftTop = _Trans.CenterLeftTop();

	// BitBlt : HDC�� �����ϴ� ���� �⺻���� �Լ�
	BitBlt(
		TargetDC,			// ���� ���� ���
		LeftTop.iX(),		// ���� ���� ���� ��ġ x
		LeftTop.iY(),		// ���� ���� ���� ��ġ y
		_Trans.Scale.iX(),  // ���� ���� �̹��� ũ�� x
		_Trans.Scale.iY(),  // ���� ���� �̹��� ũ�� y
		CopyDC,				// ������ �̹���
		0, 0,				// ������ �̹��� ������ x, y
		SRCCOPY);			// ���� �ɼ� (�״�� �����ؼ� ������)
}

