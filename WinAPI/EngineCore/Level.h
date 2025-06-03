#pragma once
#include "GameMode.h"

class CollisionLinkData
{
public:
	union
	{
		struct
		{
			int Left; 
			int Right;
		};
		__int64 Key; // 4����Ʈ Left + 4����Ʈ Right = Key�� ����� ��ĥ �� ����.
	};
};

// ���� : ȭ�鿡 ��µ� ��� ����(+������Ʈ)��� ī�޶� �����ϴ� Ŭ����
// Ÿ��Ʋ ����, �÷��� ����, ���� ���� �� ȭ�鿡 ��µ� ���͵��� �����Ѵ�.
class ULevel : public UObject
{
public:
	friend class U2DCollision;
	friend class USpriteRenderer; // �������� ��� ����� �� �Ѱ��ش�.
	friend class UEngineAPICore;

	ULevel();
	~ULevel();

	void LevelChangeStart();
	void LevelChangeEnd();

	void Tick(float _DeltaTime);
	void Render(float _DeltaTime);
	void Collision(float _DeltaTime);
	void Release(float _DeltaTime);

	template<typename ActorType>
	ActorType* SpawnActor()
	{
		ActorType* NewActor = new ActorType();
		AActor* ActorPtr = dynamic_cast<AActor*>(NewActor);
		ActorPtr->World = this;

		BeginPlayList.push_back(ActorPtr);
		return NewActor;
	}

	void SetCameraToMainPawn(bool _IsCameraToMainPawn)
	{
		IsCameraToMainPawn = _IsCameraToMainPawn;
	}

	void SetCameraPivot(FVector2D _Pivot)
	{
		CameraPivot = _Pivot;
	}

	void SetCameraPos(FVector2D _Pos)
	{
		CameraPos = _Pos;
	}

	FVector2D GetCameraPivot()
	{
		return CameraPivot;
	}

	FVector2D GetCameraPos()
	{
		return CameraPos;
	}

	void AddCameraPos(FVector2D _Pos)
	{
		CameraPos += _Pos;
	}

	AActor* GetPawn()
	{
		return MainPawn;
	}

	template<typename ConvertType>
	ConvertType* GetPawn()
	{
		return dynamic_cast<ConvertType*>(MainPawn);
	}

	AActor* GetGameMode()
	{
		return GameMode;
	}

	template<typename ConvertType>
	ConvertType* GetGameMode()
	{
		return dynamic_cast<ConvertType*>(GameMode);
	}

	template<typename LeftEnumType, typename RightEnumType>
	static void CollisionGroupLink(LeftEnumType _Left, RightEnumType _Right)
	{
		CollisionGroupLink(static_cast<int>(_Left), static_cast<int>(_Right));
	}

	static void CollisionGroupLink(int _Left, int _Right)
	{
		CollisionLinkData LinkData;
		LinkData.Left = _Left;
		LinkData.Right = _Right;

		for (size_t i = 0; i < CollisionLink.size(); i++)
		{
			if (CollisionLink[i].Key == LinkData.Key)
			{
				return;
			}
		}

		CollisionLink.push_back(LinkData);
	}

	// �ش��ϴ� ���� ��ü�� ��� ��ȯ
	template<typename ActorType>
	std::list<ActorType*> GetActorsFromClass()
	{
		std::list<ActorType*> Result;

		std::list<AActor*>::iterator StartIter = AllActors.begin();
		std::list<AActor*>::iterator EndIter = AllActors.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			AActor* CurActor = *StartIter;

			ActorType* ConvertActor = dynamic_cast<ActorType*>(CurActor);

			if (nullptr == ConvertActor)
			{
				continue;
			}

			Result.push_back(ConvertActor);
		}
		return Result;
	}

protected:

private:
	void ScreenClear();
	void DoubleBuffering();
	void BeginPlayCheck();

	template<typename GameModeType, typename MainPawnType>
	void CreateGameMode()
	{
		GameMode = new GameModeType();
		MainPawn = new MainPawnType();

		// ULevel�� ������ ���� '����'��
		MainPawn->World = this;
		GameMode->World = this;

		// �ʱ� ����
		BeginPlayList.push_back(GameMode);
		BeginPlayList.push_back(MainPawn);
	}

	// �������� ���������� �Ժη� ȣ������ ���ϰ� �ϱ� ���ؼ� private�� �־�� �Ѵ�.
	void PushRenderer(class USpriteRenderer* _Renderer);
	void ChangeRenderOrder(class USpriteRenderer* _Renderer, int _PrevOrder);
	void PushCollision(class U2DCollision* _Collision);

	void PushCheckCollision(class U2DCollision* _Collision);
	void CollisionEventCheck(class U2DCollision* _Left, class U2DCollision* _Right);

	AGameMode* GameMode = nullptr;

	AActor* MainPawn = nullptr;
	std::list<AActor*> AllActors;

	std::list<AActor*> BeginPlayList;

	// �������Ѵ�. : ���ʿ��� ������ ���� �ʵ��� Key ���� int�� map, ������� �������
	std::map<int, std::list<class USpriteRenderer*>> Renderers;

	// �浹ü �׷�
	// ����ȣ���� ���ؼ� ��� �ִ� �뵵
	std::map<int, std::list<class U2DCollision*>> Collisions;

	// �̺�Ʈ üũ����� ���ؼ� � �׷��� � �׷�� �浹���� ����� ���� �ڷᱸ��
	static std::vector<CollisionLinkData> CollisionLink;

	// �����Ӹ��� �浹üũ�� �ϴ� �ݸ������� ���� ��� ���� �ڷᱸ�� => �̰� ���ϰ� �ʹ�.
	std::map<int, std::list<class U2DCollision*>> CheckCollisions;

	// ī�޶�
	bool IsCameraToMainPawn = true;
	FVector2D CameraPos;
	FVector2D CameraPivot;

	// delete Function
	ULevel(const ULevel& _Other) = delete;
	ULevel(ULevel&& _Other) noexcept = delete;
	ULevel& operator=(const ULevel& _Other) = delete;
	ULevel& operator=(ULevel&& _Other) noexcept = delete;
};

