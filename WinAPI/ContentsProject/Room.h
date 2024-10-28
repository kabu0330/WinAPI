#pragma once
#include <EngineCore/Actor.h>

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

	ARoom* LinkRoom(ARoom* _Room, RoomDir _Dir);
protected:

private:
	ARoom* Room[4] = { 0, };
};

