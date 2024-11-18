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
		__int64 Key; // 4바이트 Left + 4바이트 Right = Key는 절대로 겹칠 수 없다.
	};
};

// 설명 : 화면에 출력될 모든 액터(+컴포넌트)들과 카메라를 관리하는 클래스
// 타이틀 레벨, 플레이 레벨, 엔딩 레벨 등 화면에 출력될 액터들을 구성한다.
class ULevel : public UObject
{
public:
	friend class U2DCollision;
	friend class USpriteRenderer; // 렌더러의 모든 기능을 다 넘겨준다.
	friend class UEngineAPICore;

	// constructer destructer
	ULevel();
	~ULevel();

	// delete Function
	ULevel(const ULevel& _Other) = delete;
	ULevel(ULevel&& _Other) noexcept = delete;
	ULevel& operator=(const ULevel& _Other) = delete;
	ULevel& operator=(ULevel&& _Other) noexcept = delete;

	// 내가 CurLevel 됐을대
	void LevelChangeStart();

	// 나 이제 새로운 레벨로 바뀔거야.
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
		// NewActor->BeginPlay();
		// AllActors.push_back(NewActor);
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

protected:

private:
	void ScreenClear();
	void DoubleBuffering();
	void BeginPlayCheck();

	template<typename GameModeType, typename MainPawnType>
	void CreateGameMode()
	{
		// 게임의 관리와 규칙을 설정할 게임 모드
		GameMode = new GameModeType();

		// 플레이어
		MainPawn = new MainPawnType();

		// ULevel이 유저가 보는 '월드'임
		MainPawn->World = this;
		GameMode->World = this;

		// 초기 세팅
		BeginPlayList.push_back(GameMode);
		BeginPlayList.push_back(MainPawn);

		//GameMode->BeginPlay();
		//MainPawn->BeginPlay();
		//AllActors.push_back(GameMode);
		//AllActors.push_back(MainPawn);
	}

	// 렌더러는 컨텐츠에서 함부로 호출하지 못하게 하기 위해서 private에 있어야 한다.
	void PushRenderer(class USpriteRenderer* _Renderer);
	void ChangeRenderOrder(class USpriteRenderer* _Renderer, int _PrevOrder);
	void PushCollision(class U2DCollision* _Collision);

	void PushCheckCollision(class U2DCollision* _Collision);
	void CollisionEventCheck(class U2DCollision* _Left, class U2DCollision* _Right);

	AGameMode* GameMode = nullptr;

	AActor* MainPawn = nullptr;
	std::list<AActor*> AllActors;

	std::list<AActor*> BeginPlayList;

	// 오더링한다. : 불필요한 정렬을 하지 않도록 Key 값이 int인 map, 만들어진 순서대로
	std::map<int, std::list<class USpriteRenderer*>> Renderers;

	// 충돌체 그룹
	// 직접호출을 위해서 들고만 있는 용도
	std::map<int, std::list<class U2DCollision*>> Collisions;

	// 이벤트 체크방식을 위해서 어떤 그룹이 어떤 그룹과 충돌하지 기록해 놓은 자료구조
	static std::vector<CollisionLinkData> CollisionLink;

	// 프레임마다 충돌체크를 하는 콜리전들을 따로 모아 놓은 자료구조 => 이거 피하고 싶다.
	std::map<int, std::list<class U2DCollision*>> CheckCollisions;

	// 카메라
	bool IsCameraToMainPawn = true;
	FVector2D CameraPos;
	FVector2D CameraPivot;
};

