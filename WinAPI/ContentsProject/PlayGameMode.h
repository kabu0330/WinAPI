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


