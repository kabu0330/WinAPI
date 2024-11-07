#include "PreCompile.h"
#include "ImageManager.h"

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EnginePath.h>
#include <EngineBase/EngineString.h>
#include <EngineBase/EngineFile.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineCore/EngineAPICore.h>

UImageManager::UImageManager()
{
}

UImageManager::~UImageManager()
{
	{
		std::map<std::string, UEngineWinImage*>::iterator StartIter = Images.begin();
		std::map<std::string, UEngineWinImage*>::iterator EndIter = Images.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			UEngineWinImage* CurRes = StartIter->second;
			if (nullptr != CurRes)
			{
				delete CurRes;
				CurRes = nullptr;
			}
		}
	}

	{
		std::map<std::string, UEngineSprite*>::iterator StartIter = Sprites.begin();
		std::map<std::string, UEngineSprite*>::iterator EndIter = Sprites.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			UEngineSprite* CurRes = StartIter->second;
			if (nullptr != CurRes)
			{
				delete CurRes;
				CurRes = nullptr;
			}
		}
	}
}

// 경로 전체를 입력하면 리소스를 로드해주는 함수
void UImageManager::Load(std::string_view Path)
{
	UEnginePath EnginePath = UEnginePath(Path);
	
	// 경로에서 리소스 파일 '이름 + 확장자명'까지만 분리해서 가져온다
	std::string FileName = EnginePath.GetFileName();

	Load(FileName, Path);
}

void UImageManager::Load(std::string_view _KeyName, std::string_view Path)
{
	UEnginePath EnginePath = UEnginePath(Path);

	if (true == EnginePath.IsDirectory())
	{
		MSGASSERT("디렉토리는 로드할수 없습니다." + std::string(Path));
		return;
	}

	if (false == EnginePath.IsExists())
	{
		MSGASSERT("유효하지 않은 파일 경로 입니다." + std::string(Path));
		return;
	}
	
	// 메인 윈도우 HDC
	UEngineWinImage* WindowImage = UEngineAPICore::GetCore()->GetMainWindow().GetWindowImage();

	// 리소스 파일 네이밍 규칙을 모두 "대문자" 기준으로 판별한다.
	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (true == Images.contains(UpperName))
	{
		MSGASSERT("이미 로드된 이미지를 다시 로드할 수 없습니다." + UpperName);
		return;
	}

	if (true == Sprites.contains(UpperName))
	{
		MSGASSERT("이미 로드된 이미지를 다시 로드할 수 없습니다." + UpperName);
		return;
	}

	UEngineWinImage* NewImage = new UEngineWinImage();
	NewImage->Load(WindowImage, Path);

	// WinImage 클래스에 "파일명 + 확장자명"을 Key / HDC와 위치, 크기를 값으로 저장
	NewImage->SetName(UpperName);
	Images.insert({ UpperName , NewImage }); // 결국 NewImage는 HDC다. 이미지 리소스는 HDC로 저장되었다.

	UEngineSprite* NewSprite = new UEngineSprite();

	// 이미지를 자르는 기준이 되는 위치는 LeftTop이다.
	FTransform Trans;
	Trans.Location = { 0,0 }; // LeftTop
	Trans.Scale = NewImage->GetImageScale(); // 본래 이미지 크기를 저장한다.

	// HDC랑 위치, 크기를 엔진Sprite::SpriteData에 저장
	NewSprite->PushData(NewImage, Trans);

	// 다시 ImageManager 클래스에 "파일명 + 확장자명"을 Key / HDC와 위치, 크기를 값으로 저장
	NewSprite->SetName(UpperName);
	Sprites.insert({ UpperName , NewSprite });
}

// 특정 폴더 안에 있는 모든 파일들을 로드해라.
// 로드 순서는 이름순서
void UImageManager::LoadFolder(std::string_view Path)
{
	UEnginePath EnginePath = UEnginePath(Path);

	std::string DirName = EnginePath.GetDirectoryName();

	LoadFolder(DirName, Path);
}

void UImageManager::LoadFolder(std::string_view _KeyName, std::string_view _Path)
{
	UEnginePath EnginePath = UEnginePath(_Path);

	if (false == EnginePath.IsExists())
	{
		MSGASSERT("유효하지 않은 파일 경로 입니다." + std::string(_Path));
		return;
	}

	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (true == Sprites.contains(UpperName))
	{
		MSGASSERT("로드된 이미지를 또 로드할 수 없습니다." + UpperName);
		return;
	}


	UEngineSprite* NewSprite = new UEngineSprite();
	NewSprite->SetName(UpperName);
	Sprites.insert({ UpperName , NewSprite });

	// 로드하기 위해서 필요한 Window Main HDC
	UEngineWinImage* WindowImage = UEngineAPICore::GetCore()->GetMainWindow().GetWindowImage();

	UEngineDirectory Dir = _Path;
	std::vector<UEngineFile> ImageFiles = Dir.GetAllFile();
	for (size_t i = 0; i < ImageFiles.size(); i++)
	{
		std::string FilePath = ImageFiles[i].GetPathToString();
		std::string UpperFileName = UEngineString::ToUpper(ImageFiles[i].GetFileName());

		UEngineWinImage* NewImage = FindImage(UpperFileName);
		if (nullptr == NewImage)
		{
			NewImage = new UEngineWinImage();
			NewImage->SetName(UpperFileName);
			NewImage->Load(WindowImage, FilePath);
		}
		Images.insert({ UpperFileName,  NewImage });

		FTransform Transform;
		Transform.Location = { 0, 0 };
		Transform.Scale = NewImage->GetImageScale();

		NewSprite->PushData(NewImage, Transform);
	}
}

void UImageManager::CuttingSprite(std::string_view _KeyName, int _X, int _Y)
{
	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (false == Sprites.contains(UpperName))
	{
		MSGASSERT("존재하지 않은 스프라이트를 자르려고 했습니다" + std::string(_KeyName));
		return;
	}

	if (false == Images.contains(UpperName))
	{
		MSGASSERT("존재하지 않은 이미지를 기반으로 스프라이트를 자르려고 했습니다" + std::string(_KeyName));
		return;
	}

	UEngineSprite* Sprite = Sprites[UpperName];
	UEngineWinImage* Image = Images[UpperName];

	Sprite->ClearSpriteData();

	FVector2D Scale = Image->GetImageScale();

	Scale.X /= _X;
	Scale.Y /= _Y;

	CuttingSprite(_KeyName, Scale);
}

void UImageManager::CuttingSprite(std::string_view _KeyName, FVector2D _CuttingSize)
{
	// 리소스 파일 네이밍 규칙을 모두 "대문자" 기준으로 판별한다.
	// 파일명 + 확장자를 대문자로 반환
	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (false == Sprites.contains(UpperName))
	{
		MSGASSERT("존재하지 않은 스프라이트는 CuttingSpriate 할 수 없습니다." + std::string(_KeyName));
		return;
	}

	if (false == Images.contains(UpperName))
	{
		MSGASSERT("존재하지 않은 이미지는 CuttingSpriate 할 수 없습니다." + std::string(_KeyName));
		return;
	}

	// C++ 20이상 최신 문법. iterator 안쓰고 key값으로 find하여 그 값을 반환
	UEngineSprite* Sprite = Sprites[UpperName];
	UEngineWinImage* Image = Images[UpperName];

	// 기존에 가지고 있던 스프라이트 이미지 지워.
	Sprite->ClearSpriteData();

	Sprite->SetName(UpperName);
	Image->SetName(UpperName);

	if (0 != (Image->GetImageScale().iX() % _CuttingSize.iX()))
	{
		MSGASSERT("스프라이트 컷팅의 x값이 0으로 나눠지지 않습니다. " + std::string(_KeyName));
		return;
	}

	if (0 != (Image->GetImageScale().iY() % _CuttingSize.iY()))
	{
		MSGASSERT("스프라이트 컷팅의 y값이 0으로 나눠지지 않습니다. " + std::string(_KeyName));
		return;
	}

	// 이미지를 크기대로 나눈다. 
	int SpriteX = Image->GetImageScale().iX() / _CuttingSize.iX();
	int SpriteY = Image->GetImageScale().iY() / _CuttingSize.iY();

	FTransform CuttingTrans;

	CuttingTrans.Location = FVector2D::ZERO;
	CuttingTrans.Scale = _CuttingSize;

	for (size_t y = 0; y < SpriteY; ++y)
	{
		for (size_t x = 0; x < SpriteX; ++x)
		{
			// Sprite::SpriteData에 저장
			Sprite->PushData(Image, CuttingTrans);
			CuttingTrans.Location.X += _CuttingSize.X;
		}

		CuttingTrans.Location.X = 0.0f;
		CuttingTrans.Location.Y += _CuttingSize.Y;
	}
}

void UImageManager::CuttingSprite(std::string_view _NewSpriteName, std::string_view _Image, FVector2D _CuttingSize)
{
	std::string SpriteUpperName = UEngineString::ToUpper(_NewSpriteName);
	std::string ImageUpperName = UEngineString::ToUpper(_Image);

	if (false == Images.contains(ImageUpperName))
	{
		MSGASSERT("존재하지 않은 이미지를 기반으로 스프라이트를 자르려고 했습니다" + std::string(_Image));
		return;
	}

	UEngineSprite* Sprite = new UEngineSprite();

	if (false == Sprites.contains(SpriteUpperName))
	{
		Sprite = new UEngineSprite();;
		Sprites.insert({ SpriteUpperName, Sprite });
	}
	else {
		Sprite = Sprites[SpriteUpperName];
	}

	UEngineWinImage* Image = Images[ImageUpperName];

	Sprite->ClearSpriteData();
	Sprite->SetName(SpriteUpperName);
	Image->SetName(ImageUpperName);

	int SpriteX = Image->GetImageScale().iX() / _CuttingSize.iX();
	int SpriteY = Image->GetImageScale().iY() / _CuttingSize.iY();

	FTransform CuttingTrans;

	CuttingTrans.Location = FVector2D::ZERO;
	CuttingTrans.Scale = _CuttingSize;

	for (size_t y = 0; y < SpriteY; ++y)
	{
		for (size_t x = 0; x < SpriteX; ++x)
		{
			Sprite->PushData(Image, CuttingTrans);
			CuttingTrans.Location.X += _CuttingSize.X;
		}

		CuttingTrans.Location.X = 0.0f;
		CuttingTrans.Location.Y += _CuttingSize.Y;
	}
}

// 기존의 이미지를 찾아 잘라낸 후 새로운 스프라이트 이미지를 만듭니다.
// _NewSpriteKeyName : 새로운 키 값
// _StartPos : 기존 이미지에서 잘라낼 좌상단 값
// _CuttingSize : 이미지 사이즈
// _XYOffSet : 이미지 사이의 빈공간 간격, XY
// _Xcount : 가로 이미지 갯수
// _ImageCount : 총 이미지 갯수
void UImageManager::CreateCutSprite(std::string_view _SearchKeyName, std::string_view _NewSpriteKeyName, FVector2D _StartPos, FVector2D _CuttingSize, FVector2D _XYOffSet, UINT _Xcount, UINT _ImageCount)
{
	std::string SearchName = UEngineString::ToUpper(_SearchKeyName);
	std::string NewSpriteName = UEngineString::ToUpper(_NewSpriteKeyName);

	if (_Xcount <= 0)
	{
		MSGASSERT("이미지의 가로 갯수가 0 이하입니다.");
		return;
	}
	if (_ImageCount <= 0)
	{
		MSGASSERT("총 이미지 갯수가 0 이하입니다.");
		return;
	}
	if (Sprites.contains(SearchName) == false)
	{
		MSGASSERT(std::string(_SearchKeyName) + "라는 이름의 Sprite는 로드할 수 없습니다.");
		return;
	}
	if (Images.contains(SearchName) == false)
	{
		MSGASSERT(std::string(_SearchKeyName) + "라는 이름의 Sprite는 로드할 수 없습니다.");
		return;
	}
	if (Sprites.contains(NewSpriteName) == true)
	{
		MSGASSERT(std::string(_NewSpriteKeyName) + "라는 이름의 Sprite가 이미 존재합니다.");
		return;
	}
	if (Images.contains(NewSpriteName) == true)
	{
		MSGASSERT(std::string(_NewSpriteKeyName) + "라는 이름의 Image가 이미 존재합니다.");
		return;
	}


	UEngineSprite* Sprite = Sprites[SearchName];
	UEngineWinImage* Image = Images[SearchName];

	Sprite->ClearSpriteData();

	UINT YCount = _ImageCount / _Xcount;
	if (_ImageCount % _Xcount > 0)
		++YCount;

	float TotalSizeX = _StartPos.X + (_CuttingSize.X * _Xcount) + (_XYOffSet.X * (_Xcount - 1));
	float TotalSizeY = _StartPos.Y + (_CuttingSize.Y * YCount) + (_XYOffSet.Y * (YCount - 1));

	if (TotalSizeX > Image->GetImageScale().X)
	{
		MSGASSERT("필요한 이미지 가로 사이즈가 원본 이미지 사이즈보다 큽니다.");
		return;
	}
	if (TotalSizeY > Image->GetImageScale().Y)
	{
		MSGASSERT("필요한 이미지 세로 사이즈가 원본 이미지 사이즈보다 큽니다.");
		return;
	}

	FVector2D TotalSize = FVector2D(static_cast<int>(TotalSizeX), static_cast<int>(TotalSizeY));


	UEngineWinImage* NewImage = new UEngineWinImage();
	UEngineSprite* NewSprite = new UEngineSprite();
	NewImage->Create(UEngineAPICore::GetCore()->GetMainWindow().GetWindowImage(), TotalSize);

	BitBlt(NewImage->GetDC(), 0, 0, static_cast<int>(TotalSizeX), static_cast<int>(TotalSizeY), Image->GetDC(), static_cast<int>(_StartPos.X), static_cast<int>(_StartPos.Y), SRCCOPY);

	Images.insert(make_pair(NewSpriteName, NewImage));
	Sprites.insert(make_pair(NewSpriteName, NewSprite));

	FVector2D CuttingPos = {};

	for (UINT y = 0; y < YCount; ++y)
	{
		CuttingPos.Y = _CuttingSize.Y * y + _XYOffSet.Y * y;

		for (UINT x = 0; x < _Xcount; ++x)
		{
			CuttingPos.X = _CuttingSize.X * x + _XYOffSet.X * x;
			FTransform insertInst = {};
			insertInst.Scale = _CuttingSize;
			insertInst.Location = CuttingPos;
			NewSprite->PushData(NewImage, insertInst);
		}
		CuttingPos.X = 0.f;
	}
}

bool UImageManager::IsLoadSprite(std::string_view _KeyName)
{
	std::string UpperName = UEngineString::ToUpper(_KeyName);

	return Sprites.contains(UpperName);
}

UEngineSprite* UImageManager::FindSprite(std::string_view _KeyName)
{
	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (false == Sprites.contains(UpperName))
	{
		MSGASSERT("로드하지 않은 스프라이트를 사용하려고 했습니다" + std::string(_KeyName));
		return nullptr;
	}

	// std::map<>::iterator find 함수의 리턴값
	return Sprites[UpperName];
}

UEngineWinImage* UImageManager::FindImage(std::string_view _KeyName)
{
	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (false == Images.contains(UpperName))
	{
		MSGASSERT("로드하지 않은 스프라이트를 사용하려고 했습니다" + std::string(_KeyName));
		return nullptr;
	}

	return Images[UpperName];
}
