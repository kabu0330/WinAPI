#pragma once
#include <EngineCore/Actor.h>
#include <map>

enum class RoomDir
{
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	MAX
};

// 설명 : 스테이지 하나의 맵
class ARoom : public AActor
{
public:
	// constrcuter destructer
	ARoom();
	~ARoom();

	// delete Function
	ARoom(const ARoom& _Other) = delete;
	ARoom(ARoom&& _Other) noexcept = delete;
	ARoom& operator=(const ARoom& _Other) = delete;
	ARoom& operator=(ARoom&& _Other) noexcept = delete;

	bool IsLinking(ARoom* _Room);
	bool InterLinkRoom(ARoom* _Room, RoomDir _Dir);
	ARoom* LinkRoom(ARoom* _Room, RoomDir _Dir);

protected:

private:
	//ARoom* Rooms[static_cast<int>(RoomDir::MAX)] = { nullptr, };
	//RoomDir RoomDir = RoomDir::NONE;
	std::map<RoomDir, ARoom*> Rooms;
};

