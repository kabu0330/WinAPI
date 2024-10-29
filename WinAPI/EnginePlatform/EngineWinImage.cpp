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
		MSGASSERT("Main WindowDC를 매개변수로 넣지 않고 이미지를 생성할 수 없습니다.");
		return;
	}

	HBITMAP NewBitMap = static_cast<HBITMAP>(CreateCompatibleBitmap(_TargetImage->GetDC(), _Scale.iX(), _Scale.iY()));
	HDC NewImageDC = CreateCompatibleDC(_TargetImage->GetDC());

	// HDC를 생성만해도 (1, 1) 이미지와 연결되어 있어, (1, 1) 이미지를 먼저 메모리 할당을 해제해준다.
	// 이로써 새로 생성한 NewImageDC는 연결된 이미지가 없는(nullptr) 상태가 된다?
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
		MSGASSERT("복사받을 대상(_TargetImage)이 존재하지 않습니다.");
		return;
	}

	HDC CopyDC = ImageDC;
	HDC TargetDC = _TargetImage->ImageDC;

	FVector2D LeftTop = _Trans.CenterLeftTop();

	// BitBlt : HDC를 복사하는 가장 기본적인 함수
	BitBlt(
		TargetDC,			// 복사 받을 대상
		LeftTop.iX(),		// 복사 받을 시작 위치 x
		LeftTop.iY(),		// 복사 받을 시작 위치 y
		_Trans.Scale.iX(),  // 복사 받을 이미지 크기 x
		_Trans.Scale.iY(),  // 복사 받을 이미지 크기 y
		CopyDC,				// 복사할 이미지
		0, 0,				// 복사할 이미지 시작점 x, y
		SRCCOPY);			// 복사 옵션 (그대로 복사해서 덮어씌우기)
}

