#include "PreCompile.h"
#include "Level.h"
#include <EngineCore/EngineAPICore.h>
#include <EnginePlatform/EngineWindow.h>
#include "SpriteRenderer.h"
#include "EngineCoreDebug.h"
#include "2DCollision.h"

ULevel::ULevel()
{
}

ULevel::~ULevel()
{
	{
		// BeginPlayList �ѹ��� ü���� ���� ���ʹ� 
		// ���͵��� �� ��� �÷��� ����Ʈ�� �� �ִ�.

		std::list<AActor*>::iterator StartIter = BeginPlayList.begin();
		std::list<AActor*>::iterator EndIter = BeginPlayList.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			AActor* CurActor = *StartIter;
			delete CurActor;
		}
	}

	{
		// ������ �����ֱ�� ������ ����.
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

// ���� CurLevel ���� �� 
void ULevel::LevelChangeStart()
{
	{
		std::list<AActor*>::iterator StartIter = AllActors.begin();
		std::list<AActor*>::iterator EndIter = AllActors.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			AActor* CurActor = *StartIter;

			// ���� ������ �Ѿ�ٰ� ���� ������ �Ѿ���� ��� ������ ���͵��� ��Ƴ���.
			CurActor->LevelChangeStart();
		}
	}

	{
		// ���� ü������ �� �� BeginPlay ȣ��
		std::list<AActor*>::iterator StartIter = BeginPlayList.begin();
		std::list<AActor*>::iterator EndIter = BeginPlayList.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			AActor* CurActor = *StartIter;

			CurActor->LevelChangeStart();
		}
	}
}

// �� ���� ���ο� ������ �ٲ�ž�.
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

	{
		std::list<AActor*>::iterator StartIter = BeginPlayList.begin();
		std::list<AActor*>::iterator EndIter = BeginPlayList.end();

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

		// ���� ��
		AActor::ComponentBeginPlay();
	}

	{
		std::list<AActor*>::iterator StartIter = AllActors.begin();
		std::list<AActor*>::iterator EndIter = AllActors.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			AActor* CurActor = *StartIter;

			// ���Ͱ� ������ Tick�� ������ �ʴ´�.
			if (false == CurActor->IsActive())
			{
				continue;
			}

			CurActor->Tick(_DeltaTime);
		}
	}
}

// ���⼭ ��� Actor���� �����Ѵ�.
void ULevel::Render(float _DeltaTime)
{
	// BackBuffer ������ 1 : ȭ�� �����
	ScreenClear();

	if (true == IsCameraToMainPawn)
	{
		//CameraPivot = { -960.0f * 0.5f, -538.0f * 0.5f };
		CameraPos = MainPawn->GetTransform().Location + CameraPivot;
	}

	// ���Ͱ� SpriteRenderer�� ����� Level�� �������� ���縦 �˾ƾ� �Ѵ�.
	// Level�� �������� ���縦 �ƴ� �� �ƴ�, �������� Level���� �ڽ��� ���縦 �˸��� ���ҷ� �����Ѵ�.
	std::map<int, std::list<class USpriteRenderer*>>::iterator StartOrderIter = Renderers.begin();
	std::map<int, std::list<class USpriteRenderer*>>::iterator EndOrderIter = Renderers.end();

	for (; StartOrderIter != EndOrderIter; ++StartOrderIter)
	{
		std::list<class USpriteRenderer*>& RendererList = StartOrderIter->second;

		std::list<class USpriteRenderer*>::iterator RenderStartIter = RendererList.begin();
		std::list<class USpriteRenderer*>::iterator RenderEndIter = RendererList.end();

		for (; RenderStartIter != RenderEndIter; ++RenderStartIter)
		{
			if (false == (*RenderStartIter)->IsActive())
			{
				continue;
			}

			(*RenderStartIter)->Render(_DeltaTime);
		}

	}

	UEngineDebug::PrintEngineDebugRender();

	// BackBuffer ������ 2 : ȭ�� �׸���
	DoubleBuffering();

	int a = 0;
}

void ULevel::Release(float _DeltaTime)
{
	// ������ ������ ���ܺ��� ������ �Ѵ�.

	{ // Collision
		std::map<int, std::list<class U2DCollision*>>::iterator StartOrderIter = Collisions.begin();
		std::map<int, std::list<class U2DCollision*>>::iterator EndOrderIter = Collisions.end();

		for (; StartOrderIter != EndOrderIter; ++StartOrderIter)
		{
			std::list<class U2DCollision*>& CollisionList = StartOrderIter->second;

			std::list<class U2DCollision*>::iterator CollisionStartIter = CollisionList.begin();
			std::list<class U2DCollision*>::iterator CollisionEndIter = CollisionList.end();

			// �𸮾��� �߰��� �����Ҽ� ����.
			for (; CollisionStartIter != CollisionEndIter; )
			{
				if (false == (*CollisionStartIter)->IsDestroy())
				{
					++CollisionStartIter;
					continue;
				}

				// �������� ���� �ʿ䰡 �����ϴ�.
				// (*RenderStartIter) ���� ���� ������ ��������.
				// ������Ʈ�� �޸𸮸� �����Ҽ� ������ ������ ���͸� ������ �ִ�.
				CollisionStartIter = CollisionList.erase(CollisionStartIter);
			}
		}
	}

	{ // Renderer
		std::map<int, std::list<class USpriteRenderer*>>::iterator StartOrderIter = Renderers.begin();
		std::map<int, std::list<class USpriteRenderer*>>::iterator EndOrderIter = Renderers.end();

		for (; StartOrderIter != EndOrderIter; ++StartOrderIter)
		{
			std::list<class USpriteRenderer*>& RendererList = StartOrderIter->second;

			std::list<class USpriteRenderer*>::iterator RenderStartIter = RendererList.begin();
			std::list<class USpriteRenderer*>::iterator RenderEndIter = RendererList.end();

			// �𸮾�� �޸� �߰��� �������� ������ �� �ִ�.
			for (; RenderStartIter != RenderEndIter; )
			{
				if (false == (*RenderStartIter)->IsDestroy())
				{
					++RenderStartIter;
					continue;
				}

				// �������� ���� �ʿ䰡 �����ϴ�.
				// (*RenderStartIter) ���� ���� ������ ��������.
				// ������Ʈ�� �޸𸮸� �����Ҽ� ������ ������ ���͸� ������ �ִ�.
				RenderStartIter = RendererList.erase(RenderStartIter);
			}
		}
	}
	{
		std::list<AActor*>::iterator StartIter = AllActors.begin();
		std::list<AActor*>::iterator EndIter = AllActors.end();

		for (; StartIter != EndIter; )
		{
			AActor* CurActor = *StartIter;


			if (false == CurActor->IsDestroy())
			{
				CurActor->ReleaseCheck(_DeltaTime);
				++StartIter;
				continue;
			}

			// ������ ������ ���������� ������ ������ ���ʹ� ��¥ ���� �ش�.
			delete CurActor;
			StartIter = AllActors.erase(StartIter);
		}
	}
}

// ȭ���� �����ϰ� ȭ��Ʈ�� �����.
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
	// Renderers�� ������ SpriteRenderer�� �˰� �ְ� �ȴ�.
	// �̸� ���� ���͵��� ���� ������ ���� �� �ִ�.
	int Order = _Renderer->GetOrder();

	Renderers[Order].push_back(_Renderer);
}

void ULevel::PushCollision(U2DCollision* _Collision)
{
	int Order = _Collision->GetGroup();
	Collisions[Order].push_back(_Collision);
}

void ULevel::ChangeRenderOrder(USpriteRenderer* _Renderer, int _PrevOrder)
{
	//std::vector<int> Value;
	// ���ʹ� �����갡 ����.
	//Value.remove

	// 0���� ����־��� ���̴�.
	// ���� ���� �Լ��� �ƴմϴ�.
	Renderers[_PrevOrder].remove(_Renderer);

	Renderers[_Renderer->GetOrder()].push_back(_Renderer);


}


