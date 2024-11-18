#pragma once
#include <EngineCore/Actor.h>
#include <map>
#include <vector>
#include "Global.h"
#include <EngineCore/SpriteRenderer.h>
#include "Monster.h"

enum class RoomDir
{
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	MAX
};

// 설명 : 플레이 레벨에 존재하는 맵 
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

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	bool IsLinking(ARoom* _Room);
	bool InterLinkRoom(ARoom* _Room, RoomDir _Dir);
	ARoom* LinkRoom(ARoom* _Room, RoomDir _Dir);
	void AddDoor(RoomDir _Dir, ARoom* ConnectedRoom);

	void DoorSpriteSetting();
	void SpriteSetting();
	void CollisionSetting();

	void WarpCollisionCheck(float _DeltaTime);
	void WarpPlayerSetting();
	void Warp(float _DeltaTime);

	class U2DCollision* GetRoomCollision()
	{
		return RoomCollision;
	}

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

	bool IsCameraMove()
	{
		return CameraMove;
	}

	float GetRoomSizeOffsetX()
	{
		return RoomSizeOffsetX;
	}

	float GetRoomSizeOffsetY()
	{
		return RoomSizeOffsetY;
	}

	template<typename MonsterType>
	AMonster* CreateMonster(FVector2D _Pivot)
	{
		AMonster* NewMonster = GetWorld()->SpawnActor<MonsterType>();
		NewMonster->SetActorLocation(this->GetActorLocation() + _Pivot);
		NewMonster->SetParentRoom(this);
		Monsters.push_back(NewMonster);

		return NewMonster;
	}
	void RemoveMonster(AMonster* _Monster)
	{
		Monsters.remove(_Monster);
	}

	int CountFly();

	//template<typename MonsterType>
	//std::list<AMonster*> CurRoomFindMonsters(MonsterType _MonsterType)
	//{
	//	std::list<AMonster*>::iterator StartIter = Monsters.begin();
	//	std::list<AMonster*>::iterator EndIter = Monsters.end();
	//	std::list<AMonster*> ReturnMonsters;

	//	for (; StartIter != EndIter; ++StartIter)
	//	{
	//		AMonster* Monster = *StartIter;
	//		ARoom ParentRoom = Monster->ParentRoom;

	//		if (ParentRoom != GetCurRoom())
	//		{
	//			continue;
	//		}

	//		AMonster* FindMonster = dynamic_cast<_MonsterType>(Monster);
	//		if (nullptr == FindMonster)
	//		{
	//			ReturnMonsters.push_back(FindMonster);
	//		}
	//	}

	//	return ReturnMonsters;
	//}

protected:

private:
	static ARoom* CurRoom;

	std::map<RoomDir, ARoom*> Rooms;
	std::map<RoomDir, U2DCollision*> DoorCollisionMap;
	std::map<RoomDir, USpriteRenderer*> DoorRendererMap;
	
	RoomDir Directon = RoomDir::NONE;
	FVector2D RoomScale = FVector2D::ZERO;

	float RoomSizeOffsetX = 0.0f;
	float RoomSizeOffsetY = 0.0f;

	// Room Collision And Renderer
	class U2DCollision* RoomCollision = nullptr;

	USpriteRenderer* RoomRenderer       = nullptr; // 임시 방 하나 생성
	USpriteRenderer* ControlsRenderer   = nullptr; // BaseRoom 컨트롤러 이미지

	// Door Collision And Renderer
	std::vector<class U2DCollision*> DoorCollisions;
	std::vector<USpriteRenderer*> DoorRenderers;

	// MonsterSpawn
	std::list<AMonster*> Monsters;

	// 카메라 이동관련 멤버
	AActor* Player = nullptr;
	float CameraMoveTime = 0.0f;
	float LerpAlpha = 0.0f;
	bool  CameraMove = false;
	FVector2D TargetPlayerPos = FVector2D::ZERO;
	FVector2D CameraMoveDir = FVector2D::ZERO;
	FVector2D StartCameraPos = FVector2D::ZERO;
	FVector2D EndCameraPos = FVector2D::ZERO;
	RoomDir MoveDir = RoomDir::NONE;
};

