#include "PreCompile.h"
#include "EngineWinImage.h"
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EnginePath.h>
#include <EngineBase/EngineString.h>

// GDI PLUS�� ���
#include <objidl.h>  // .bmp
#include <gdiplus.h> // .png

#pragma comment(lib, "Msimg32.lib") // bmp Ȯ��� ���̺귯��
#pragma comment(lib, "Gdiplus.lib") // png�� ���� ������ ����Ƽ�� �׷��� Ȯ��� ���̺귯��

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

// ����� ������ ���� ���� ������ �Ѵ�.
void UEngineWinImage::Create(UEngineWinImage* _TargetImage, FVector2D _Scale)
{										//  ������ ���� HDC
	if (nullptr == _TargetImage)
	{
		MSGASSERT("Main WindowDC�� �Ű������� ���� �ʰ� �̹����� ������ �� �����ϴ�.");
		return;
	}

	HBITMAP NewBitMap = static_cast<HBITMAP>(CreateCompatibleBitmap(_TargetImage->GetDC(), _Scale.iX(), _Scale.iY()));
	HDC NewImageDC = CreateCompatibleDC(_TargetImage->GetDC());

	// HDC�� �������ص� (1, 1) �̹����� ����Ǿ� �־�, (1, 1) �̹����� ���� �޸� �Ҵ��� �������ش�.
	// �̷ν� ���� ������ NewImageDC�� ����� �̹����� ����(nullptr) ���°� �ȴ�
	HBITMAP OldBitMap = static_cast<HBITMAP>(SelectObject(NewImageDC, NewBitMap));
	DeleteObject(OldBitMap);

	hBitMap = NewBitMap;
	ImageDC = NewImageDC;

	// hBitMap�� ����(�ʺ�, ���� ��)�� Info�� ���� / GetObejct : �����ϸ� ����Ʈ �� ��ȯ, �����ϸ� 0 ��ȯ
	GetObject(hBitMap, sizeof(BITMAP), &Info);
}

// ����� ������ HDC �Լ�. �̹��� ������ BitBlt �Լ��� �������� �ʴ�.
void UEngineWinImage::CopyToBit(UEngineWinImage* _TargetImage, const FTransform& _Trans)
{
	if (nullptr == _TargetImage)
	{
		MSGASSERT("������� ���(_TargetImage)�� �������� �ʽ��ϴ�.");
		return;
	}

	HDC CopyDC = ImageDC; // BackBuffer
	HDC TargetDC = _TargetImage->ImageDC; // WinMainHDC

	FVector2D LeftTop = _Trans.CenterLeftTop();

	// BitBlt : HDC�� �����ϴ� ���� �⺻���� �Լ�
	BitBlt(
		TargetDC,			// ���� ���� ��� : ������ â
		LeftTop.iX(),		// ���� ���� ���� ��ġ x
		LeftTop.iY(),		// ���� ���� ���� ��ġ y
		_Trans.Scale.iX(),  // ���� ���� �̹��� ũ�� x
		_Trans.Scale.iY(),  // ���� ���� �̹��� ũ�� y
		CopyDC,				// ������ �̹��� : BackBuffer
		0, 0,				// ������ �̹��� ������ x, y
		SRCCOPY);			// ���� �ɼ� (�״�� �����ؼ� ������)
}

// �̹��� ������ HDC �Լ�, BitBlt���� �̹��� ũ�� ����, �������� ������ ������ �� �־� �����(Alpha)�� ���� bmp���Ͽ� �����ϴ�. 
void UEngineWinImage::CopyToTrans(UEngineWinImage* _TargetImage, const FTransform& _RenderTrans, const FTransform& _LTImageTrans, UColor _Color /*= UColor(255, 0, 255, 0)*/)
{
	HDC CopyDC = ImageDC; // BackBuffer
	HDC TargetDC = _TargetImage->ImageDC; // WinMainHDC

	FVector2D LeftTop = _RenderTrans.CenterLeftTop();

	TransparentBlt(
		TargetDC,					 // Window Main DC : ���� ȭ�鿡 ���� â
		LeftTop.iX(),				 // �׸��� ������ X ��ǥ : 0
		LeftTop.iY(),				 // �׸��� ������ Y ��ǥ : 0
		_RenderTrans.Scale.iX(),	 // ������ �׸��� X ��ǥ : ������ ȭ�� �ʺ�
		_RenderTrans.Scale.iY(),	 // ������ �׸��� Y ��ǥ : ������ ȭ�� ����
		CopyDC,						 // ������ ��� : ���� ���ҽ�(�̹���)
		_LTImageTrans.Location.iX(), // ���縦 ������ X ��ǥ : 0
		_LTImageTrans.Location.iY(), // ���縦 ������ Y ��ǥ : 0
		_LTImageTrans.Scale.iX(),	 // ������ �������� X ��ǥ : ���� ������ ũ��
		_LTImageTrans.Scale.iY(),	 // ������ �������� Y ��ǥ : ���� ������ ũ��
		_Color.Color				 // �������� ���� : �⺻��(����Ÿ)
	);
}

// bmp�� ��ȯ �� �̹��� ���� ���� �� �̹����� �ε��� HDC�� HBITMAP �Ҵ� �� ����
void UEngineWinImage::Load(UEngineWinImage* _TargetImage, std::string_view _Path)
{						// _TargetImage : ������ ���� HDC	
	// png ���ϰ� bmp ������ �ε� ����� �ٸ���.
	// bmp�� �����찡 �����ϴ� �̹��� ���� �����̰�, png�� �׷��� �ʴ�.
	// �׷��� DX������ GDI PLUS�� Ȱ���� png ���ϵ� ȣȯ �����ϰ� �������� �� �ִµ� ������ �ִ�.
	// 1. �̹��� ó�� �ӵ��� ���� ��ü �����ӿ� ��û�� �ǿ����� �� ������ ������
	// 2. �׷���ī�带 Ȱ���ϴ� ����� ���� DX�� ������� �ʰ� WinAPI�� �����ϴ� ������ ����ȴ�.
	// �׷��� GDI PLUS�δ� png ������ �ε��ϴµ� ������ Ȱ���ϰ� bmp ���Ϸ� ��ȯ�Ͽ� ���İ��� ����������. 

	UEnginePath Path = _Path;

	// Ȯ���ڸ� �����Ͽ� �빮�ڷ� ��ȯ
	std::string UpperExt = UEngineString::ToUpper(Path.GetExtension());

	HBITMAP NewBitmap = nullptr;

	// PNG
	if (".PNG" == UpperExt)
	{
		ULONG_PTR GdiPlusToken = 0; // GdiPlus�� �ڵ��� ǥ���� �� ���
		
		// �ʱ� ����
		Gdiplus::GdiplusStartupInput StartupInput;
		Gdiplus::GdiplusStartup(&GdiPlusToken, &StartupInput, nullptr);

		// ���̵����Ʈ�� ����
		std::wstring WidePath = UEngineString::AnsiToUnicode(_Path);

		// ��θ� �ָ� png�̹����� �ε����ִ� �Լ�
		Gdiplus::Image* pImage = Gdiplus::Image::FromFile(WidePath.c_str());

		// png �̹��� ������ ���纻�� �����ϰ� bitmap*���� ���� ����ȯ
		Gdiplus::Bitmap* pBitMap = reinterpret_cast<Gdiplus::Bitmap*>(pImage->Clone());

		// Bitmap���� ��ȯ �� �۾��� �����ߴ��� Status enum���� ��ȯ
		// GetHBITMAP : ���� ����(����Ÿ), ��ȯ�� ��Ʈ�� ������
		Gdiplus::Status Stat = pBitMap->GetHBITMAP(Gdiplus::Color(255, 255, 0, 255), &NewBitmap);

		// HBITMAP ���⿡ �����ϸ�
		if (Gdiplus::Status::Ok != Stat)
		{
			MSGASSERT("png���� �̹��� �ε忡 �����߽��ϴ�." + std::string(_Path));
			return;
		}

		// ���� �޸𸮸� ��������� �Ѵ�.
		delete pBitMap;
		delete pImage;
	}


	// ��Ʈ��
	else if (".BMP" == UpperExt)
	{
		HANDLE NewHandle = LoadImageA(nullptr, _Path.data(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		NewBitmap = reinterpret_cast<HBITMAP>(NewHandle);
	}


	// ��Ʈ�� ��ȯ�� �����ϸ�
	if (nullptr == NewBitmap)
	{
		MSGASSERT("�̹��� �ε忡 �����߽��ϴ�. " + std::string(_Path));
		return;
	}

	// ������ â HDC�� ȣȯ ������ HDC�� ����
	HDC NewImageDC = CreateCompatibleDC(_TargetImage->GetDC());

	HBITMAP OldBitMap = static_cast<HBITMAP>(SelectObject(NewImageDC, NewBitmap));
	DeleteObject(OldBitMap); // (1, 1) ¥�� �̹��� DC�� ����

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

	// ::�� �ٿ��� �Ѵ�. ��������� ������ �����ϴ� ������
	// UEngineWinImage::GetPixel ȥ���� �ü� �ִ�.

	// RGBA
	// 1111
	UColor ResultColor = ::GetPixel(ImageDC, _Point.X, _Point.Y);
	return ResultColor;
}

