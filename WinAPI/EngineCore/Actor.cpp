#include "PreCompile.h"
#include "Actor.h"

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineMath.h>
#include <EnginePlatform/EngineWindow.h>
#include <EnginePlatform/EngineWinImage.h>
#include <EngineCore/EngineAPICore.h>

#include "EngineSprite.h"
#include "ImageManager.h"

AActor::AActor()
{
}

AActor::~AActor()
{
}

void AActor::Render()
{
	if (nullptr == Sprite)
	{
		MSGASSERT("스프라이트가 세팅되지 않은 액터를 랜더링을 할수 없습니다.");
		return;
	}

	UEngineWindow& MainWindow = UEngineAPICore::GetCore()->GetMainWindow();
	UEngineWinImage* BackBufferImage = MainWindow.GetBackBuffer();


	UEngineSprite::USpriteData CurData = Sprite->GetSpriteData(CurIndex);
	CurData.Image;
	CurData.Transform;
	CurData.Image->CopyToTrans(BackBufferImage, Transform, CurData.Transform);

	////                  100 100  - 50 50 => 50 50
	//FVector2D LeftTop = Transform.Location - Transform.Scale.Half();
	////                  100 100  + 50 50 => 150 150
	//FVector2D RightBot = Transform.Location + Transform.Scale.Half();



	//// MainWindowDc에 그리면 그려지는 애들의 영역이 겹칠경우에는 깜박이는 경합현상이 벌어진다.
	//// HDC의 특정 영역을 다투는 것이다 2개의 랜더링
 //  //Rectangle(MainWindow.GetWindowMainDC(), LeftTop.iX(), LeftTop.iY(), RightBot.iX(), RightBot.iY());

 //  // 
	//UEngineWindow& MainWindow = UEngineAPICore::GetCore()->GetMainWindow();
	//UEngineWinImage* BackBufferImage = MainWindow.GetBackBuffer();
	//HDC BackBufferDC = BackBufferImage->GetDC();
	//Rectangle(BackBufferDC, LeftTop.iX(), LeftTop.iY(), RightBot.iX(), RightBot.iY());

}

void AActor::SetSprite(std::string_view _Name, int _CurIndex /*= 0*/)
{
	// 싱글톤에 대해서 설명할때
	// 값을 편하게 공유하기 위해서 사용하는 거라고 하면 틀렸다.
	// 객체를 단 1개 만드는 패턴이라는 것을 잊지 마시고

	// 액터가 만들어졌을때는 로드가 끝난 상황이어야 한다.
	Sprite = UImageManager::GetInst().FindSprite(_Name);

	if (nullptr == Sprite)
	{
		MSGASSERT("로드하지 않은 스프라이트를 사용하려고 했습니다" + std::string(_Name));
		return;
	}

	CurIndex = _CurIndex;
}
