#pragma once
#include <EngineCore/Actor.h>
#include <map>
#include <vector>
#include "Global.h"

enum class RoomDir
{
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	MAX
};

// ���� : �������� �ϳ��� ��
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

	void AddDoor(FVector2D _RoomLocation, RoomDir _Dir, ARoom* ConnectedRoom);

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	FVector2D GetLeftPos()
	{
		int PosX = GetActorLocation().iX();
		int HalfScaleX = GetActorScale().Half().iX();
		int Result = PosX - HalfScaleX; 
		return { Result, GetActorLocation().iY()};
	}

	FVector2D GetRightPos()
	{
		int PosX = GetActorLocation().iX();
		int HalfScaleX = GetActorScale().Half().iX();
		int Result = PosX + HalfScaleX;
		return { Result, GetActorLocation().iY() };
	}

	FVector2D GetUpPos()
	{
		int PosY = GetActorLocation().iY();
		int HalfScaleY = GetActorScale().Half().iY();
		int Result = PosY - HalfScaleY;
		return { GetActorLocation().iX() , Result };
	}

	FVector2D GetDownPos()
	{
		int PosY = GetActorLocation().iY();
		int HalfScaleY = GetActorScale().Half().iY();
		int Result = PosY + HalfScaleY;
		return { GetActorLocation().iX() , Result };
	}

	ARoom* GetConnectedRoom(RoomDir _Direction);

protected:

private:
	std::map<RoomDir, ARoom*> Rooms;
	RoomDir Directon = RoomDir::NONE;

	std::map<RoomDir, class ADoor*> Doors;

	class USpriteRenderer* RoomRenderer = nullptr; // �ӽ� �� �ϳ� ����
	class USpriteRenderer* BolderLineRenderer = nullptr;
};

