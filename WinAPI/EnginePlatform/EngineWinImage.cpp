#include "PreCompile.h"
#include "EngineWinImage.h"
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EnginePath.h>
#include <EngineBase/EngineString.h>

// GDI PLUS용 헤더
#include <objidl.h>  // .bmp
#include <gdiplus.h> // .png

#pragma comment(lib, "Msimg32.lib") // bmp 확장용 라이브러리
#pragma comment(lib, "Gdiplus.lib") // png를 통한 윈도우 네이티브 그래픽 확장용 라이브러리

UEngineWinImage::UEngineWinImage()
{
}

UEngineWinImage::~UEngineWinImage()
{
	if (nullptr != hBitMap)
	{
		DeleteObject(hBitMap);
		hBitMap = nullptr;
	}

	if (nullptr != ImageDC)
	{
		DeleteDC(ImageDC);
		ImageDC = nullptr;
	}
}

// 백버퍼 생성을 위한 최초 세팅을 한다.
void UEngineWinImage::Create(UEngineWinImage* _TargetImage, FVector2D _Scale)
{										//  윈도우 메인 HDC
	if (nullptr == _TargetImage)
	{
		MSGASSERT("Main WindowDC를 매개변수로 넣지 않고 이미지를 생성할 수 없습니다.");
		return;
	}

	HBITMAP NewBitMap = static_cast<HBITMAP>(CreateCompatibleBitmap(_TargetImage->GetDC(), _Scale.iX(), _Scale.iY()));
	HDC NewImageDC = CreateCompatibleDC(_TargetImage->GetDC());

	// HDC를 생성만해도 (1, 1) 이미지와 연결되어 있어, (1, 1) 이미지를 먼저 메모리 할당을 해제해준다.
	// 이로써 새로 생성한 NewImageDC는 연결된 이미지가 없는(nullptr) 상태가 된다
	HBITMAP OldBitMap = static_cast<HBITMAP>(SelectObject(NewImageDC, NewBitMap));
	DeleteObject(OldBitMap);

	hBitMap = NewBitMap;
	ImageDC = NewImageDC;

	// hBitMap의 정보(너비, 높이 등)를 Info에 저장 / GetObejct : 성공하면 바이트 수 반환, 실패하면 0 반환
	GetObject(hBitMap, sizeof(BITMAP), &Info);
}

// 백버퍼 생성용 HDC 함수. 이미지 생성에 BitBlt 함수는 적절하지 않다.
void UEngineWinImage::CopyToBit(UEngineWinImage* _TargetImage, const FTransform& _Trans)
{
	if (nullptr == _TargetImage)
	{
		MSGASSERT("복사받을 대상(_TargetImage)이 존재하지 않습니다.");
		return;
	}

	HDC CopyDC = ImageDC; // BackBuffer
	HDC TargetDC = _TargetImage->ImageDC; // WinMainHDC

	FVector2D LeftTop = _Trans.CenterLeftTop();

	// BitBlt : HDC를 복사하는 가장 기본적인 함수
	BitBlt(
		TargetDC,			// 복사 받을 대상 : 윈도우 창
		LeftTop.iX(),		// 복사 받을 시작 위치 x
		LeftTop.iY(),		// 복사 받을 시작 위치 y
		_Trans.Scale.iX(),  // 복사 받을 이미지 크기 x
		_Trans.Scale.iY(),  // 복사 받을 이미지 크기 y
		CopyDC,				// 복사할 이미지 : BackBuffer
		0, 0,				// 복사할 이미지 시작점 x, y
		SRCCOPY);			// 복사 옵션 (그대로 복사해서 덮어씌우기)
}

// 이미지 생성용 HDC 함수, BitBlt보다 이미지 크기 조정, 날려버릴 색상을 지정할 수 있어 투명색(Alpha)이 없는 bmp파일에 적합하다. 
void UEngineWinImage::CopyToTrans(UEngineWinImage* _TargetImage, const FTransform& _RenderTrans, const FTransform& _LTImageTrans, UColor _Color /*= UColor(255, 0, 255, 0)*/)
{
	HDC CopyDC = ImageDC; // BackBuffer
	HDC TargetDC = _TargetImage->ImageDC; // WinMainHDC

	FVector2D LeftTop = _RenderTrans.CenterLeftTop();

	TransparentBlt(
		TargetDC,					 // Window Main DC : 실제 화면에 보일 창
		LeftTop.iX(),				 // 그리기 시작할 X 좌표 : 0
		LeftTop.iY(),				 // 그리기 시작할 Y 좌표 : 0
		_RenderTrans.Scale.iX(),	 // 어디까지 그릴지 X 좌표 : 윈도우 화면 너비
		_RenderTrans.Scale.iY(),	 // 어디까지 그릴지 Y 좌표 : 윈도우 화면 높이
		CopyDC,						 // 복사할 대상 : 나의 리소스(이미지)
		_LTImageTrans.Location.iX(), // 복사를 시작할 X 좌표 : 0
		_LTImageTrans.Location.iY(), // 복사를 시작할 Y 좌표 : 0
		_LTImageTrans.Scale.iX(),	 // 어디까지 복사할지 X 좌표 : 내가 설정한 크기
		_LTImageTrans.Scale.iY(),	 // 어디까지 복사할지 Y 좌표 : 내가 설정한 크기
		_Color.Color				 // 날려버릴 배경색 : 기본값(마젠타)
	);
}

// bmp로 변환 후 이미지 정보 저장 및 이미지를 로드할 HDC와 HBITMAP 할당 및 저장
void UEngineWinImage::Load(UEngineWinImage* _TargetImage, std::string_view _Path)
{						// _TargetImage : 윈도우 메인 HDC	
	// png 파일과 bmp 파일의 로드 방식이 다르다.
	// bmp는 윈도우가 지원하는 이미지 파일 형식이고, png는 그렇지 않다.
	// 그래서 DX이전에 GDI PLUS를 활용해 png 파일도 호환 가능하게 지원해줄 수 있는데 문제가 있다.
	// 1. 이미지 처리 속도가 게임 전체 프레임에 엄청난 악영향을 줄 정도로 느리고
	// 2. 그래픽카드를 활용하는 기술을 쓰면 DX를 사용하지 않고 WinAPI를 공부하는 목적에 위배된다.
	// 그래서 GDI PLUS로는 png 파일을 로드하는데 까지만 활용하고 bmp 파일로 변환하여 알파값을 날려버린다. 

	UEnginePath Path = _Path;

	// 확장자명만 추출하여 대문자로 변환
	std::string UpperExt = UEngineString::ToUpper(Path.GetExtension());

	HBITMAP NewBitmap = nullptr;

	// PNG
	if (".PNG" == UpperExt)
	{
		ULONG_PTR GdiPlusToken = 0; // GdiPlus용 핸들을 표현할 때 사용
		
		// 초기 설정
		Gdiplus::GdiplusStartupInput StartupInput;
		Gdiplus::GdiplusStartup(&GdiPlusToken, &StartupInput, nullptr);

		// 와이드바이트로 변경
		std::wstring WidePath = UEngineString::AnsiToUnicode(_Path);

		// 경로를 주면 png이미지를 로딩해주는 함수
		Gdiplus::Image* pImage = Gdiplus::Image::FromFile(WidePath.c_str());

		// png 이미지 파일의 복사본을 생성하고 bitmap*으로 강제 형변환
		Gdiplus::Bitmap* pBitMap = reinterpret_cast<Gdiplus::Bitmap*>(pImage->Clone());

		// Bitmap으로 변환 후 작업이 성공했는지 Status enum으로 반환
		// GetHBITMAP : 배경색 설정(마젠타), 반환할 비트맵 포인터
		Gdiplus::Status Stat = pBitMap->GetHBITMAP(Gdiplus::Color(255, 255, 0, 255), &NewBitmap);

		// HBITMAP 추출에 실패하면
		if (Gdiplus::Status::Ok != Stat)
		{
			MSGASSERT("png파일 이미지 로드에 실패했습니다." + std::string(_Path));
			return;
		}

		// 직접 메모리를 해제해줘야 한다.
		delete pBitMap;
		delete pImage;
	}


	// 비트맵
	else if (".BMP" == UpperExt)
	{
		HANDLE NewHandle = LoadImageA(nullptr, _Path.data(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		NewBitmap = reinterpret_cast<HBITMAP>(NewHandle);
	}


	// 비트맵 변환에 실패하면
	if (nullptr == NewBitmap)
	{
		MSGASSERT("이미지 로드에 실패했습니다. " + std::string(_Path));
		return;
	}

	// 윈도우 창 HDC와 호환 가능한 HDC를 생성
	HDC NewImageDC = CreateCompatibleDC(_TargetImage->GetDC());

	HBITMAP OldBitMap = static_cast<HBITMAP>(SelectObject(NewImageDC, NewBitmap));
	DeleteObject(OldBitMap); // (1, 1) 짜리 이미지 DC는 해제

	hBitMap = NewBitmap;
	ImageDC = NewImageDC;

	GetObject(hBitMap, sizeof(BITMAP), &Info);
}

UColor UEngineWinImage::GetColor(FIntPoint _Point, UColor _DefaultColor = UColor::WHITE)
{
	if (0 > _Point.X)
	{
		return _DefaultColor;
	}

	if (0 > _Point.Y)
	{
		return _DefaultColor;
	}

	if (Info.bmWidth <= _Point.X)
	{
		return _DefaultColor;
	}

	if (Info.bmHeight <= _Point.Y)
	{
		return _DefaultColor;
	}

	// ::꼭 붙여야 한다. 명시적으로 전역에 존재하는 윈도우
	// UEngineWinImage::GetPixel 혼돈이 올수 있다.

	// RGBA
	// 1111
	UColor ResultColor = ::GetPixel(ImageDC, _Point.X, _Point.Y);
	return ResultColor;
}

