#pragma once
#include <EngineCore/GameMode.h>
#include <vector>
#include "Room.h"

// 설명 : Play 레벨에서 가장 먼저 세팅할 것들을 입력하는 클래스
class APlayGameMode : public AGameMode
{
public:
	// constrcuter destructer
	APlayGameMode();
	~APlayGameMode();

	// delete Function
	APlayGameMode(const APlayGameMode& _Other) = delete;
	APlayGameMode(APlayGameMode&& _Other) noexcept = delete;
	APlayGameMode& operator=(const APlayGameMode& _Other) = delete;
	APlayGameMode& operator=(APlayGameMode&& _Other) noexcept = delete;

	void BeginPlay();
	void Tick(float _DeltaTime);

	void CollisionGroupLinkSetting();
	void UISetting();
	void Spawn();
	void SpawnRandomItem(ARoom* _ParentRoom);

	void EngineDebug(float _DeltaTime);

	void LevelChangeStart() override;

	void FadeOut();
	void CheckInput();

	void SwitchIsShowDeathReport()
	{
		IsShowMenu = !IsShowMenu;
	}
	
	static bool IsGamePaused()
	{
		return true == GamePaused;
	}
	static void SetGamePaused(bool _OnOff)
	{
		GamePaused = _OnOff;
	}

	static USoundPlayer& GetPlayGameModeBGM()
	{
		PlayGameModeBGM.Loop(999);
		return PlayGameModeBGM;
	}

protected:

private:
	void RandomIndex();
	
	class ALoadingScene* LoadingImage = nullptr;
	bool IsShowMenu = false;
	static bool GamePaused; // 게임 일시정지

	UEngineRandom Random;
	int Index = 0;
	bool IsSwap = false;
	int RandomValues[7] = { 0, };

	static USoundPlayer PlayGameModeBGM;
	USoundPlayer Sound;
	bool IsPlayingBGM = false;
};


