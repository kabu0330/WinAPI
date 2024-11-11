#pragma once
#include <EngineCore/Actor.h>
#include <map>
#include <vector>
#include "Global.h"
#include <EngineCore/SpriteRenderer.h>

enum class RoomDir
{
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	MAX
};

// ���� : �÷��� ������ �����ϴ� �� 
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
	void DoorSpriteSetting();

	void AddDoor(RoomDir _Dir, ARoom* ConnectedRoom);

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

protected:

private:
	std::map<RoomDir, ARoom*> Rooms;
	RoomDir Directon = RoomDir::NONE;
	FVector2D RoomScale = FVector2D::ZERO;

	// Room Collision And Renderer
	class U2DCollision* RoomCollision = nullptr;

	USpriteRenderer* RoomRenderer       = nullptr; // �ӽ� �� �ϳ� ����
	USpriteRenderer* BolderLineRenderer = nullptr; // ȭ�� �����ڸ� ���� ���
	USpriteRenderer* ControlsRenderer   = nullptr; // BaseRoom ��Ʈ�ѷ� �̹���

	// Door Collision And Renderer
	std::vector<class U2DCollision*> DoorCollisions;
	std::vector<USpriteRenderer*> DoorRenderers;
	

	// ī�޶� �̵����� ���
	AActor* Player = nullptr;
	FVector2D PlayerPos = FVector2D::ZERO;
	float CameraMoveTime = 0.0f;
	float LerpAlpha = 0.0f;
	bool  CameraMove = false;
	FVector2D TargetPlayerPos = FVector2D::ZERO;
	FVector2D CameraMoveDir = FVector2D::ZERO;
	FVector2D StartCameraPos = FVector2D::ZERO;
	FVector2D EndCameraPos = FVector2D::ZERO;
	ESpriteDir MoveDir = ESpriteDir::NONE;
};

