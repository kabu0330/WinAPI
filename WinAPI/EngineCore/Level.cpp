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
}

void ULevel::Collision(float _DeltaTime)
{
	for (size_t i = 0; i < CollisionLink.size(); i++)
	{
		CollisionLinkData Data = CollisionLink[i];

		int Left = Data.Left;
		int Right = Data.Right;

		// �̺�Ʈ�� �浹üũ�ϴ� �׷�
		std::list<class U2DCollision*>& LeftList = CheckCollisions[Left];

		// �� ����� �̺�Ʈ �׷��� �ƴϾ �ǹǷ� �׳� �ݸ��� �������� �����´�.
		std::list<class U2DCollision*>& RightList = Collisions[Right];

		std::list<class U2DCollision*>::iterator StartLeftIter = LeftList.begin();
		std::list<class U2DCollision*>::iterator EndLeftIter = LeftList.end();

		std::list<class U2DCollision*>::iterator StartRightIter = RightList.begin();
		std::list<class U2DCollision*>::iterator EndRightIter = RightList.end();

		for (; StartLeftIter != EndLeftIter; ++StartLeftIter)
		{
			U2DCollision* LeftCollision = *StartLeftIter;

			if (false == LeftCollision->IsActive())
			{
				continue;
			}

			for (; StartRightIter != EndRightIter; ++StartRightIter)
			{
				U2DCollision* RightCollision = *StartRightIter;
				if (false == RightCollision->IsActive())
				{
					continue;
				}

				LeftCollision->CollisionEventCheck(RightCollision);
			}
		}
	}
}

void ULevel::Release(float _DeltaTime)
{
	// ������ ������ ���ܺ��� ������ �Ѵ�.

	// �浹ü ����
	{
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

				CollisionStartIter = CollisionList.erase(CollisionStartIter);
			}
		}
	}

	// �̺�Ʈ �浹ü ����
	{
		std::map<int, std::list<class U2DCollision*>>::iterator StartOrderIter = CheckCollisions.begin();
		std::map<int, std::list<class U2DCollision*>>::iterator EndOrderIter = CheckCollisions.end();

		for (; StartOrderIter != EndOrderIter; ++StartOrderIter)
		{
			std::list<class U2DCollision*>& CollisionList = StartOrderIter->second;

			std::list<class U2DCollision*>::iterator CollisionStartIter = CollisionList.begin();
			std::list<class U2DCollision*>::iterator CollisionEndIter = CollisionList.end();

			for (; CollisionStartIter != CollisionEndIter; )
			{
				if (false == (*CollisionStartIter)->IsDestroy())
				{
					++CollisionStartIter;
					continue;
				}

				CollisionStartIter = CollisionList.erase(CollisionStartIter);
			}
		}
	}

	// ������ ����
	{
		std::map<int, std::list<class USpriteRenderer*>>::iterator StartOrderIter = Renderers.begin();
		std::map<int, std::list<class USpriteRenderer*>>::iterator EndOrderIter = Renderers.end();

		for (; StartOrderIter != EndOrderIter; ++StartOrderIter)
		{
			std::list<class USpriteRenderer*>& RendererList = StartOrderIter->second;

			std::list<class USpriteRenderer*>::iterator RenderStartIter = RendererList.begin();
			std::list<class USpriteRenderer*>::iterator RenderEndIter = RendererList.end();

			for (; RenderStartIter != RenderEndIter; )
			{
				if (false == (*RenderStartIter)->IsDestroy())
				{
					++RenderStartIter;
					continue;
				}

				// �������� ������ ���� �ʿ䰡 ����.
				// (*RenderStartIter) ���� ���� ������ ��������.
				// ������Ʈ�� �޸𸮸� �����Ҽ� ������ ������ ���͸� ������ �ִ�.
				RenderStartIter = RendererList.erase(RenderStartIter);
			}
		}
	}

	// ���� ����
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

std::vector<CollisionLinkData> ULevel::CollisionLink;

void ULevel::PushCheckCollision(class U2DCollision* _Collision)
{
	int Order = _Collision->GetGroup();
	CheckCollisions[Order].push_back(_Collision);
}

void ULevel::ChangeRenderOrder(USpriteRenderer* _Renderer, int _PrevOrder)
{
	//std::vector<int> Value;
	// ���ʹ� �����갡 ����.
	//Value.remove

	// 0���� ����־��� ���̴�.
	Renderers[_PrevOrder].remove(_Renderer);

	Renderers[_Renderer->GetOrder()].push_back(_Renderer);
}


