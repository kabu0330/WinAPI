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


	void CollisionGroupLinkSetting();
	void BeginPlay();

	void Tick(float _DeltaTime);

	void UISetting();

	void EngineDebug(float _DeltaTime);

protected:

private:
};


