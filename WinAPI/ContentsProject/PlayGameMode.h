#pragma once
#include <EngineCore/GameMode.h>
#include <vector>
#include "Room.h"

// ���� : Play �������� ���� ���� ������ �͵��� �Է��ϴ� Ŭ����
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


	void EngineDebug(float _DeltaTime);

	void LevelChangeStart() override;

	void FadeOut();
	void FadeActiveFalse();
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

protected:

private:
	class ALoadingScene* LoadingImage = nullptr;
	bool IsShowMenu = false;
	static bool GamePaused; // ���� �Ͻ�����
};


