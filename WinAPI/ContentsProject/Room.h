#pragma once
#include <EngineCore/Actor.h>
#include <map>
#include <vector>
#include "Global.h"
#include <EngineCore/SpriteRenderer.h>
#include "Monster.h"
#include "RoomObject.h"
#include "Item.h"

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

	void OpenTheDoor();
	void CloseTheDoor();
	std::string SwitchEnumToString(RoomDir _Dir);
	void TemporarilyHideDoorCollision();

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
	int GetAliveMonsterCount();

	int CountFly();

	template<typename ObjectType>
	ARoomObject* CreateObject(FVector2D _Pivot = {0, 0})
	{
		ARoomObject* NewObject = GetWorld()->SpawnActor<ObjectType>();
		NewObject->SetActorLocation(this->GetActorLocation() + _Pivot);
		NewObject->SetParentRoom(this);
		Objects.push_back(NewObject);

		return NewObject;
	}
	void RemoveObject(ARoomObject* _Object)
	{
		Objects.remove(_Object);
	}

	template<typename ItemType>
	AItem* CreateItem(AItem* _Item = nullptr, FVector2D _Pivot = {0, 0})
	{
		AItem* NewItem = GetWorld()->SpawnActor<ItemType>();
		if (nullptr == _Item)
		{
			NewItem->SetActorLocation(this->GetActorLocation() + _Pivot);
		}
		else
		{
			NewItem->SetActorLocation(_Item->GetActorLocation() + _Pivot);
		}
		
		NewItem->SetParentRoom(this);
		Items.push_back(NewItem);

		return NewItem;
	}
	void RemoveItem(AItem* _Item)
	{
		Items.remove(_Item);
	}

protected:

private:
	static ARoom* CurRoom;

	std::map<RoomDir, ARoom*> Rooms;
	std::map<RoomDir, U2DCollision*> DoorCollisionMap;
	std::map<RoomDir, USpriteRenderer*> DoorRendererMap;
	
	RoomDir Directon = RoomDir::NONE;
	FVector2D RoomScale = FVector2D::ZERO;
	FVector2D DoorCollisionScale = FVector2D::ZERO;

	float RoomSizeOffsetX = 0.0f;
	float RoomSizeOffsetY = 0.0f;

	// Room Collision And Renderer
	class U2DCollision* RoomCollision = nullptr;
	class U2DCollision* ClampTearCollision = nullptr;

	USpriteRenderer* RoomRenderer       = nullptr; // 임시 방 하나 생성
	USpriteRenderer* ControlsRenderer   = nullptr; // BaseRoom 컨트롤러 이미지

	// Door Collision And Renderer
	std::vector<class U2DCollision*> DoorCollisions;
	std::vector<USpriteRenderer*> DoorRenderers;

	// Spawn
	std::list<AMonster*> Monsters;
	std::list<ARoomObject*> Objects;
	std::list<AItem*> Items;

	bool IsWarp = false; // 잠깐 워프 불가능하게 충돌체를 숨긴다.

	// 카메라 이동관련 멤버
	APlayer* Player = nullptr;
	float CameraMoveTime = 0.0f;
	float LerpAlpha = 0.0f;
	bool  CameraMove = false;
	FVector2D TargetPlayerPos = FVector2D::ZERO;
	FVector2D CameraMoveDir = FVector2D::ZERO;
	FVector2D StartCameraPos = FVector2D::ZERO;
	FVector2D EndCameraPos = FVector2D::ZERO;
	RoomDir MoveDir = RoomDir::NONE;
};

