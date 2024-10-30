#include "PreCompile.h"
#include "ImageManager.h"

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EnginePath.h>
#include <EngineBase/EngineString.h>
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


	UEngineWinImage* NewImage = new UEngineWinImage();
	NewImage->Load(WindowImage, Path);

	// WinImage 클래스에 "파일명 + 확장자명"을 Key / HDC와 위치, 크기를 값으로 저장
	Images.insert({ UpperName , NewImage }); // 결국 NewImage는 HDC다. 이미지 리소스는 HDC로 저장되었다.

	UEngineSprite* NewSprite = new UEngineSprite();

	// 이미지를 자르는 기준이 되는 위치는 LeftTop이다.
	FTransform Trans;
	Trans.Location = { 0,0 }; // LeftTop
	Trans.Scale = NewImage->GetImageScale();

	// HDC랑 위치, 크기를 엔진Sprite::SpriteData에 저장
	NewSprite->PushData(NewImage, Trans);

	// 다시 ImageManager 클래스에 "파일명 + 확장자명"을 Key / HDC와 위치, 크기를 값으로 저장
	Sprites.insert({ UpperName , NewSprite });
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
