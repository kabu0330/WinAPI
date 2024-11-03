#include "PreCompile.h"
#include "Level.h"
#include <EngineCore/EngineAPICore.h>
#include <EnginePlatform/EngineWindow.h>
#include "SpriteRenderer.h"
#include "EngineCoreDebug.h"

ULevel::ULevel()
{
}

ULevel::~ULevel()
{
	{
		// BeginPlayList 한번도 체인지 안한 액터는 
		// 액터들이 다 비긴 플레이 리스트에 들어가 있다.

		std::list<AActor*>::iterator StartIter = BeginPlayList.begin();
		std::list<AActor*>::iterator EndIter = BeginPlayList.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			AActor* CurActor = *StartIter;
			delete CurActor;
		}
	}

	{
		// 액터의 생명주기는 레벨과 같다.
		std::list<AActor*>::iterator StartIter = AllActors.begin();
		std::list<AActor*>::iterator EndIter = AllActors.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			AActor* CurActor = *StartIter;

			if (nullptr != CurActor)
			{
				delete CurActor;
			}
		}
	}
}

// 내가 CurLevel 됐을 때 
void ULevel::LevelChangeStart()
{
	{
		std::list<AActor*>::iterator StartIter = AllActors.begin();
		std::list<AActor*>::iterator EndIter = AllActors.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			AActor* CurActor = *StartIter;

			CurActor->LevelChangeStart();
		}
	}

}

// 나 이제 새로운 레벨로 바뀔거야.
void ULevel::LevelChangeEnd()
{
	{
		std::list<AActor*>::iterator StartIter = AllActors.begin();
		std::list<AActor*>::iterator EndIter = AllActors.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			AActor* CurActor = *StartIter;

			CurActor->LevelChangeEnd();
		}
	}

}

void ULevel::Tick(float _DeltaTime)
{
	{
		std::list<AActor*>::iterator StartIter = BeginPlayList.begin();
		std::list<AActor*>::iterator EndIter = BeginPlayList.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			AActor* CurActor = *StartIter;
			CurActor->BeginPlay();
			AllActors.push_back(CurActor);
		}

		BeginPlayList.clear();

		// 생성 시
		AActor::ComponentBeginPlay();
	}

	{
		std::list<AActor*>::iterator StartIter = AllActors.begin();
		std::list<AActor*>::iterator EndIter = AllActors.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			AActor* CurActor = *StartIter;

			CurActor->Tick(_DeltaTime);
		}
	}
}

// 여기서 모든 Actor들을 렌더한다.
void ULevel::Render(float _DeltaTime)
{
	// BackBuffer 렌더링 1 : 화면 지우기
	ScreenClear();

	if (true == IsCameraToMainPawn)
	{
		//CameraPivot = { -960.0f * 0.5f, -538.0f * 0.5f };
		CameraPos = MainPawn->GetTransform().Location + CameraPivot;
	}

	// 액터가 SpriteRenderer를 만들면 Level도 렌더러의 존재를 알아야 한다.
	// Level이 렌더러의 존재를 아는 게 아닌, 렌더러가 Level에게 자신의 존재를 알리는 역할로 렌더한다.
	std::map<int, std::list<class USpriteRenderer*>>::iterator StartOrderIter = Renderers.begin();
	std::map<int, std::list<class USpriteRenderer*>>::iterator EndOrderIter = Renderers.end();

	for (; StartOrderIter != EndOrderIter; ++StartOrderIter)
	{
		std::list<class USpriteRenderer*>& RendererList = StartOrderIter->second;

		std::list<class USpriteRenderer*>::iterator RenderStartIter = RendererList.begin();
		std::list<class USpriteRenderer*>::iterator RenderEndIter = RendererList.end();

		for (; RenderStartIter != RenderEndIter; ++RenderStartIter)
		{
			(*RenderStartIter)->Render(_DeltaTime);
		}

	}

	UEngineDebug::PrintEngineDebugText();

	// BackBuffer 렌더링 2 : 화면 그리기
	DoubleBuffering();
}

// 화면을 깨끗하게 화이트로 지운다.
void ULevel::ScreenClear()
{
	UEngineWindow& MainWindow = UEngineAPICore::GetCore()->GetMainWindow();
	UEngineWinImage* BackBufferImage = MainWindow.GetBackBuffer();
	FVector2D Size = MainWindow.GetWindowSize();

	Rectangle(BackBufferImage->GetDC(), 0, 0, Size.iX(), Size.iY());
}

void ULevel::DoubleBuffering()
{

	UEngineWindow& MainWindow = UEngineAPICore::GetCore()->GetMainWindow();

	UEngineWinImage* WindowImage = MainWindow.GetWindowImage();
	UEngineWinImage* BackBufferImage = MainWindow.GetBackBuffer();

	FTransform Trans;
	Trans.Location = MainWindow.GetWindowSize().Half();
	Trans.Scale = MainWindow.GetWindowSize();

	BackBufferImage->CopyToBit(WindowImage, Trans);
}

void ULevel::PushRenderer(USpriteRenderer* _Renderer)
{
	// Renderers의 순번은 SpriteRenderer가 알고 있게 된다.
	// 이를 통해 액터들의 렌더 순서를 정할 수 있다.
	int Order = _Renderer->GetOrder();

	Renderers[Order].push_back(_Renderer);
}

void ULevel::ChangeRenderOrder(USpriteRenderer* _Renderer, int _PrevOrder)
{
	//std::vector<int> Value;
	// 벡터는 리무브가 없다.
	//Value.remove

	// 0번에 들어있었을 것이다.
	// 별로 빠른 함수는 아닙니다.
	Renderers[_PrevOrder].remove(_Renderer);

	Renderers[_Renderer->GetOrder()].push_back(_Renderer);


}

