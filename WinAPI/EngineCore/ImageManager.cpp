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

// ��� ��ü�� �Է��ϸ� ���ҽ��� �ε����ִ� �Լ�
void UImageManager::Load(std::string_view Path)
{
	UEnginePath EnginePath = UEnginePath(Path);
	
	// ��ο��� ���ҽ� ���� '�̸� + Ȯ���ڸ�'������ �и��ؼ� �����´�
	std::string FileName = EnginePath.GetFileName();

	Load(FileName, Path);
}

void UImageManager::Load(std::string_view _KeyName, std::string_view Path)
{
	UEnginePath EnginePath = UEnginePath(Path);

	if (true == EnginePath.IsDirectory())
	{
		MSGASSERT("���丮�� �ε��Ҽ� �����ϴ�." + std::string(Path));
		return;
	}

	if (false == EnginePath.IsExists())
	{
		MSGASSERT("��ȿ���� ���� ���� ��� �Դϴ�." + std::string(Path));
		return;
	}
	
	// ���� ������ HDC
	UEngineWinImage* WindowImage = UEngineAPICore::GetCore()->GetMainWindow().GetWindowImage();

	// ���ҽ� ���� ���̹� ��Ģ�� ��� "�빮��" �������� �Ǻ��Ѵ�.
	std::string UpperName = UEngineString::ToUpper(_KeyName);


	UEngineWinImage* NewImage = new UEngineWinImage();
	NewImage->Load(WindowImage, Path);

	// WinImage Ŭ������ "���ϸ� + Ȯ���ڸ�"�� Key / HDC�� ��ġ, ũ�⸦ ������ ����
	Images.insert({ UpperName , NewImage }); // �ᱹ NewImage�� HDC��. �̹��� ���ҽ��� HDC�� ����Ǿ���.

	UEngineSprite* NewSprite = new UEngineSprite();

	// �̹����� �ڸ��� ������ �Ǵ� ��ġ�� LeftTop�̴�.
	FTransform Trans;
	Trans.Location = { 0,0 }; // LeftTop
	Trans.Scale = NewImage->GetImageScale();

	// HDC�� ��ġ, ũ�⸦ ����Sprite::SpriteData�� ����
	NewSprite->PushData(NewImage, Trans);

	// �ٽ� ImageManager Ŭ������ "���ϸ� + Ȯ���ڸ�"�� Key / HDC�� ��ġ, ũ�⸦ ������ ����
	Sprites.insert({ UpperName , NewSprite });
}

void UImageManager::CuttingSprite(std::string_view _KeyName, FVector2D _CuttingSize)
{
	// ���ҽ� ���� ���̹� ��Ģ�� ��� "�빮��" �������� �Ǻ��Ѵ�.
	// ���ϸ� + Ȯ���ڸ� �빮�ڷ� ��ȯ
	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (false == Sprites.contains(UpperName))
	{
		MSGASSERT("�������� ���� ��������Ʈ�� CuttingSpriate �� �� �����ϴ�." + std::string(_KeyName));
		return;
	}

	if (false == Images.contains(UpperName))
	{
		MSGASSERT("�������� ���� �̹����� CuttingSpriate �� �� �����ϴ�." + std::string(_KeyName));
		return;
	}

	// C++ 20�̻� �ֽ� ����. iterator �Ⱦ��� key������ find�Ͽ� �� ���� ��ȯ
	UEngineSprite* Sprite = Sprites[UpperName];
	UEngineWinImage* Image = Images[UpperName];

	// ������ ������ �ִ� ��������Ʈ �̹��� ����.
	Sprite->ClearSpriteData();

	if (0 != (Image->GetImageScale().iX() % _CuttingSize.iX()))
	{
		MSGASSERT("��������Ʈ ������ x���� 0���� �������� �ʽ��ϴ�. " + std::string(_KeyName));
		return;
	}

	if (0 != (Image->GetImageScale().iY() % _CuttingSize.iY()))
	{
		MSGASSERT("��������Ʈ ������ y���� 0���� �������� �ʽ��ϴ�. " + std::string(_KeyName));
		return;
	}

	// �̹����� ũ���� ������. 
	int SpriteX = Image->GetImageScale().iX() / _CuttingSize.iX();
	int SpriteY = Image->GetImageScale().iY() / _CuttingSize.iY();

	FTransform CuttingTrans;

	CuttingTrans.Location = FVector2D::ZERO;
	CuttingTrans.Scale = _CuttingSize;

	for (size_t y = 0; y < SpriteY; ++y)
	{
		for (size_t x = 0; x < SpriteX; ++x)
		{
			// Sprite::SpriteData�� ����
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
		MSGASSERT("�ε����� ���� ��������Ʈ�� ����Ϸ��� �߽��ϴ�" + std::string(_KeyName));
		return nullptr;
	}

	// std::map<>::iterator find �Լ��� ���ϰ�
	return Sprites[UpperName];
}
