#pragma once
#include "GameMode.h"

// 설명 : 화면에 출력될 모든 액터(+컴포넌트)들과 카메라를 관리하는 클래스
// 타이틀 레벨, 플레이 레벨, 엔딩 레벨 등 화면에 출력될 액터들을 구성한다.
class ULevel
{
public:
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

	AActor* GetPlayer()
	{
		return MainPawn;
	}

protected:

private:
	void ScreenClear();
	void DoubleBuffering();

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

	AGameMode* GameMode = nullptr;

	AActor* MainPawn = nullptr;
	std::list<AActor*> AllActors;

	std::list<AActor*> BeginPlayList;

	// 오더링한다. : 불필요한 정렬을 하지 않도록 Key 값이 int인 map, 만들어진 순서대로
	std::map<int, std::list<class USpriteRenderer*>> Renderers;

	// 카메라
	bool IsCameraToMainPawn = true;
	FVector2D CameraPos;
	FVector2D CameraPivot;
};

