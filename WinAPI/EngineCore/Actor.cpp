#include "PreCompile.h"
#include "Actor.h"
#include <EngineCore/EngineAPICore.h>

AActor::AActor()
{
}

AActor::~AActor()
{
}

void AActor::Render()
{
	//                  100 100  - 50 50 => 50 50
	FVector2D LeftTop = Transform.Location - Transform.Scale.Half();
	//                  100 100  + 50 50 => 150 150
	FVector2D RightBot = Transform.Location + Transform.Scale.Half();



	// MainWindowDc에 그리면 그려지는 애들의 영역이 겹칠경우에는 깜박이는 경합현상이 벌어진다.
	// HDC의 특정 영역을 다투는 것이다 2개의 랜더링
   //Rectangle(MainWindow.GetWindowMainDC(), LeftTop.iX(), LeftTop.iY(), RightBot.iX(), RightBot.iY());

   // 
	UEngineWindow& MainWindow = UEngineAPICore::GetCore()->GetMainWindow();
	UEngineWinImage* BackBufferImage = MainWindow.GetBackBuffer();
	HDC BackBufferDC = BackBufferImage->GetDC();
	Rectangle(BackBufferDC, LeftTop.iX(), LeftTop.iY(), RightBot.iX(), RightBot.iY());

}

