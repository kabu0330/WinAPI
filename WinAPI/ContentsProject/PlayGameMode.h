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

	static void SetCurRoom(class ARoom* _Room)
	{
		CurRoom = _Room;
	}
	static class ARoom* GetCurRoom()
	{
		return CurRoom;
	}
	
	static std::string GetCurRoomName() 
	{
		if (nullptr != CurRoom)
		{
			return CurRoom->GetName();
		}
		return "Unknown Room";
	}

	void EngineDebug(float _DeltaTime);

	void PlayerCurRoom();

protected:

private:
	static class ARoom* CurRoom;
	std::vector<class ARoom*> AllRooms;
};


