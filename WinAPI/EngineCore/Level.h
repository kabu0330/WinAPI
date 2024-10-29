#pragma once
#include "GameMode.h"

// 설명 :
class ULevel
{
public:
	friend class UEngineAPICore;

	// constructer destructer
	ULevel();
	~ULevel();

	// delete Function
	ULevel(const ULevel& _Other) = delete;
	ULevel(ULevel&& _Other) noexcept = delete;
	ULevel& operator=(const ULevel& _Other) = delete;
	ULevel& operator=(ULevel&& _Other) noexcept = delete;

	void Tick(float _DeltaTime);
	void Render();

	template<typename ActorType>
	ActorType* SpawnActor()
	{
		ActorType* NewActor = new ActorType();
		AActor* ActorPtr = dynamic_cast<AActor*>(NewActor);
		ActorPtr->World = this;

		NewActor->BeginPlay();
		AllActors.push_back(NewActor);
		return NewActor;
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
		GameMode->BeginPlay();
		MainPawn->BeginPlay();

		AllActors.push_back(GameMode);
		AllActors.push_back(MainPawn);
	}

	AGameMode* GameMode = nullptr;

	AActor* MainPawn = nullptr;
	std::list<AActor*> AllActors;
};

