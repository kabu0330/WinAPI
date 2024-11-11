#include "PreCompile.h"
#include "Room.h"

#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "ContentsEnum.h"
#include "PlayGameMode.h"
#include "Player.h"

ARoom* ARoom::CurRoom = nullptr;

ARoom::ARoom()
{
	SetActorLocation({ Global::WindowSize.Half().iX(), Global::WindowSize.Half().iY()});
	SetActorScale(Global::WindowSize);

	float ScaleX = Global::WindowSize.X / 960;
	float ScaleY = Global::WindowSize.Y / 540;
	RoomScale = { 960 * ScaleX * GetActorScale().X / Global::WindowSize.X , 540 * ScaleY * GetActorScale().Y / Global::WindowSize.Y };

	SpriteSetting();
	DoorSpriteSetting();

	CollisionSetting();

	DebugOn();
}

void ARoom::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	// Player Warp
	WarpCollisionCheck(_DeltaTime);
	Warp(_DeltaTime);

}

void ARoom::WarpCollisionCheck(float _DeltaTime)
{
	if (true == CameraMove)
	{
		return;
	}

	FTransform PlayerTrans = GetWorld()->GetPawn()->GetTransform();
	FVector2D RoomScaleA = Global::WindowSize;

	std::map<RoomDir, U2DCollision*>::iterator StartIter = DoorCollisionMap.begin();
	std::map<RoomDir, U2DCollision*>::iterator EndIter = DoorCollisionMap.end();

	for (; StartIter != EndIter ; ++StartIter)
	{
		U2DCollision* Collision = StartIter->second;
		USpriteRenderer* Renderer = DoorRendererMap[StartIter->first];

		FTransform DoorTrans = Collision->GetActorTransform();
		if (true == FTransform::RectToRect(PlayerTrans, DoorTrans))
		{
			MoveDir = StartIter->first;
			CameraMoveDir = Global::SwitchEnumToDir(MoveDir); // FVector2D:: Left, Right, Up, Down

			StartCameraPos = GetWorld()->GetCameraPos();
			EndCameraPos = GetWorld()->GetCameraPos() + RoomScaleA * CameraMoveDir;

			CameraMove = true;
			return;
		}
	}
}

void ARoom::Warp(float _DeltaTime)
{
	if (false == CameraMove)
	{
		return;
	}

	// 플레이어 위치 이동
	WarpPlayerSetting(); 

	float CameraLerpTime = 0.1f;
	LerpAlpha = FVector2D::Clamp(CameraMoveTime / CameraLerpTime, 0.0f, 1.0f);

	FVector2D CamPos = FVector2D::Lerp(StartCameraPos, EndCameraPos, LerpAlpha);
	GetWorld()->SetCameraPos(CamPos);

	CameraMoveTime += _DeltaTime;
	if (CameraLerpTime < CameraMoveTime)
	{
		CameraMoveDir = FVector2D::ZERO;

		// 카메라 이동이 완료되고도, 잠시동안 플레이어의 움직임을 제한
		float PlayerMovementCooldown = CameraLerpTime + 0.9f;
		if (PlayerMovementCooldown < CameraMoveTime)
		{
			CameraMove = false;
			CameraMoveTime = 0.0f;
			CurRoom = Rooms[MoveDir];
		}
	}
	
}

void ARoom::WarpPlayerSetting()
{
	// 플레이어는 즉시 다음 맵 문 위치로 이동
	FVector2D Offset = FVector2D::ZERO;

	switch (MoveDir)
	{
	case RoomDir::LEFT:
		Offset = -FVector2D(GetActorScale().Half().iX() + 180, 0);
		break;
	case RoomDir::RIGHT:
		Offset = FVector2D(GetActorScale().Half().iX() + 180, 0);
		break;
	case RoomDir::UP:
		Offset = -FVector2D(0, GetActorScale().Half().iY() + 110);
		break;
	case RoomDir::DOWN:
		Offset = FVector2D(0, GetActorScale().Half().iY() + 110);
		break;
	default:
		break;
	}

	TargetPlayerPos = GetActorLocation() + Offset;
	Player->SetActorLocation(TargetPlayerPos);
}

bool ARoom::IsLinking(ARoom* _Room)
{
	std::map<RoomDir, ARoom*>::iterator StartIter = Rooms.begin();
	std::map<RoomDir, ARoom*>::iterator EndIter = Rooms.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (_Room == StartIter->second)
		{
			return true;
		}
	}
	return false;
}

bool ARoom::InterLinkRoom(ARoom* _Room, RoomDir _Dir)
{
	this->LinkRoom(_Room, _Dir);
	AddDoor(_Dir, this);

	// this와 _Room의 문 위치는 서로 정반대가 되어야 한다.
	if (RoomDir::LEFT == _Dir)
	{
		_Dir = RoomDir::RIGHT;
		_Room->Directon = _Dir;
	}
	else if (RoomDir::RIGHT == _Dir)
	{
		_Dir = RoomDir::LEFT;
		_Room->Directon = _Dir;
	}
	else if (RoomDir::UP == _Dir)
	{
		_Dir = RoomDir::DOWN;
		_Room->Directon = _Dir;
	}
	else if (RoomDir::DOWN == _Dir)
	{
		_Dir = RoomDir::UP;
		_Room->Directon = _Dir;
	}

	_Room->LinkRoom(this, _Dir);

	// Room 상대 위치 조정
	if (RoomDir::LEFT == _Room->Directon)
	{
		_Room->SetActorLocation({ GetRightPos().X + GetActorScale().Half().X, GetActorLocation().Y });
	}
	else if (RoomDir::RIGHT == _Room->Directon)
	{
		_Room->SetActorLocation({ GetLeftPos().X - GetActorScale().Half().X, GetActorLocation().Y });
	}
	else if (RoomDir::UP == _Room->Directon)
	{
		_Room->SetActorLocation({ GetActorLocation().X, GetDownPos().Y + GetActorScale().Half().Y });
	}
	else if (RoomDir::DOWN == _Room->Directon)
	{
		_Room->SetActorLocation({ GetActorLocation().X, GetUpPos().Y - GetActorScale().Half().Y });
	}

	// 방향이 바뀐 위치로 방의 위치가 조정된 뒤 문 추가
	_Room->AddDoor(_Dir, _Room);

	return true;
}

ARoom* ARoom::LinkRoom(ARoom* _Room, RoomDir _Dir)
{
	if (this == _Room)
	{
		MSGASSERT("자기 자신을 연결할 수 없습니다.");
		return nullptr;
	}

	if (true == IsLinking(_Room))
	{
		MSGASSERT("이미 연결된 방입니다.");
		return nullptr;
	}

	switch (_Dir)
	{
	case RoomDir::LEFT:
	{
		Rooms.insert({ RoomDir::LEFT, _Room });	
		return Rooms.find(RoomDir::LEFT)->second;
		break;
	}
	case RoomDir::RIGHT:
	{
		Rooms.insert({ RoomDir::RIGHT, _Room });
		return Rooms.find(RoomDir::RIGHT)->second;
		break;
	}
	case RoomDir::UP:
	{
		Rooms.insert({ RoomDir::UP, _Room });
		return Rooms.find(RoomDir::UP)->second;
		break;
	}
	case RoomDir::DOWN:
	{
		Rooms.insert({ RoomDir::DOWN, _Room });
		return Rooms.find(RoomDir::DOWN)->second;
		break;
	}
	default:
		break;
	}

    return nullptr;
}

void ARoom::AddDoor(RoomDir _Dir, ARoom* _ConnectedRoom)
{	
	FVector2D DoorPos = FVector2D::ZERO;
	FVector2D DoorOffestX = FVector2D(RoomScale.Half().iX(), 0);
	FVector2D DoorOffestY = FVector2D(0, RoomScale.Half().iY());
	FVector2D OffestX = { 110, 0 };
	FVector2D OffestY = { 0, 57};

	switch (_Dir)
	{
	case RoomDir::LEFT:
		DoorPos = -1 * DoorOffestX  + OffestX;
		//DoorRenderers[0]->ChangeAnimation("Door_Left_Open");
		//DoorRenderers[0]->ChangeAnimation("Door_Left_LockAnim");
		DoorRenderers[0]->ChangeAnimation("Door_Left_OpenAnim");
		DoorRenderers[0]->SetComponentLocation(DoorPos);
		DoorRenderers[0]->SetActive(true);

		DoorCollisions[0]->SetComponentScale({ 60, 60 });
		DoorCollisions[0]->SetActive(true);

		DoorRendererMap.insert({ _Dir , DoorRenderers[0] });
		DoorCollisionMap.insert({ _Dir ,DoorCollisions[0] });
		break;
	case RoomDir::RIGHT:
		DoorPos = DoorOffestX - OffestX;
		//DoorRenderers[1]->ChangeAnimation("Door_Right_Open");
		//DoorRenderers[1]->ChangeAnimation("Door_Right_LockAnim");
		DoorRenderers[1]->ChangeAnimation("Door_Right_OpenAnim");
		DoorRenderers[1]->SetComponentLocation(DoorPos);
		DoorRenderers[1]->SetActive(true);

		DoorCollisions[1]->SetComponentScale({ 60, 60 });
		DoorCollisions[1]->SetActive(true);

		DoorRendererMap.insert({ _Dir , DoorRenderers[1] });
		DoorCollisionMap.insert({ _Dir ,DoorCollisions[1]});
		break;
	case RoomDir::UP:
		DoorPos = -1 * DoorOffestY + OffestY;
		//DoorRenderers[2]->ChangeAnimation("Door_Up_Open");
		//DoorRenderers[2]->ChangeAnimation("Door_Up_LockAnim");
		DoorRenderers[2]->ChangeAnimation("Door_Up_OpenAnim");
		DoorRenderers[2]->SetComponentLocation(DoorPos);
		DoorRenderers[2]->SetActive(true);

		DoorCollisions[2]->SetComponentScale({ 60, 60 });
		DoorCollisions[2]->SetActive(true);
		DoorRendererMap.insert({ _Dir , DoorRenderers[2] });
		DoorCollisionMap.insert({ _Dir ,DoorCollisions[2] });
		break;
	case RoomDir::DOWN:
		DoorPos = DoorOffestY - OffestY;
		//DoorRenderers[3]->ChangeAnimation("Door_Down_Open");
		//DoorRenderers[3]->ChangeAnimation("Door_Down_LockAnim");
		DoorRenderers[3]->ChangeAnimation("Door_Down_OpenAnim");
		DoorRenderers[3]->SetComponentLocation(DoorPos);
		DoorRenderers[3]->SetActive(true);

		DoorCollisions[3]->SetComponentScale({ 60, 60 });
		DoorCollisions[3]->SetActive(true);

		DoorRendererMap.insert({ _Dir , DoorRenderers[3] });
		DoorCollisionMap.insert({ _Dir ,DoorCollisions[3] });
		break;
	default:
		break;
	}

	for (int i = 0; i < DoorCollisions.size(); i++)
	{
		DoorCollisions[i]->SetComponentLocation(DoorRenderers[i]->GetComponentLocation());
	}
}

void ARoom::BeginPlay()
{
	Super::BeginPlay();

	Player = GetWorld()->GetPawn();

	// 맵 이름에 따라 이미지를 바꾼다.
	if ("BaseRoom" == GetName())
	{
		ControlsRenderer = CreateDefaultSubObject<USpriteRenderer>();
		ControlsRenderer->SetSprite("controls.png");
		ControlsRenderer->SetComponentLocation({ GetActorLocation().iX() - Global::WindowHalfScale.iX(), GetActorLocation().iY() - Global::WindowHalfScale.iY() - 20 });
		ControlsRenderer->SetComponentScale({ 655, 145 }); // 385, 85
		ControlsRenderer->SetOrder(ERenderOrder::CONTROLS);
	}
	else if ("BossRoom" == GetName())
	{
		RoomRenderer->SetSprite("Room_03.png");
	}
	else // 베이스룸을 제외한 맵의 이미지를 바꿀 수 있다.
	{
		RoomRenderer->SetSprite("Room_02.png");
	}
}

void ARoom::SpriteSetting()
{
	RoomRenderer = CreateDefaultSubObject<USpriteRenderer>();
	RoomRenderer->SetSprite("Room_01.png");
	RoomRenderer->SetComponentScale(RoomScale);
	RoomRenderer->SetOrder(ERenderOrder::BACKGROUND);
}

void ARoom::CollisionSetting()
{
	RoomCollision = CreateDefaultSubObject<U2DCollision>();
	RoomCollision->SetComponentLocation({ 0, 0 });
	RoomCollision->SetComponentScale({ RoomScale.X - 270, RoomScale.Y - 165 });
	RoomCollision->SetCollisionGroup(ECollisionGroup::OBJECT_WALL);
	RoomCollision->SetCollisionType(ECollisionType::Rect);

	DoorCollisions.resize(4);
	for (int i = 0; i < DoorCollisions.size(); i++)
	{
		DoorCollisions[i] = CreateDefaultSubObject<U2DCollision>();
		DoorCollisions[i]->SetComponentScale({ 0, 0 }); // SetActive를 false로 설정하여도 Collision이 맵 가운데에 생성되는 문제
		DoorCollisions[i]->SetCollisionGroup(ECollisionGroup::WARP);
		DoorCollisions[i]->SetCollisionType(ECollisionType::Rect);
		DoorCollisions[i]->SetActive(false);
	}
}

void ARoom::DoorSpriteSetting()
{
	USpriteRenderer* Door = nullptr;
	Door = CreateDefaultSubObject<USpriteRenderer>();
	DoorRenderers.push_back(Door);
	Door = CreateDefaultSubObject<USpriteRenderer>();
	DoorRenderers.push_back(Door);
	Door = CreateDefaultSubObject<USpriteRenderer>();
	DoorRenderers.push_back(Door);
	Door = CreateDefaultSubObject<USpriteRenderer>();
	DoorRenderers.push_back(Door);

	for (int i = 0; i < DoorRenderers.size(); i++)
	{
		DoorRenderers[i]->SetComponentScale({ 128, 128 });
		DoorRenderers[i]->SetOrder(ERenderOrder::DOOR);
		DoorRenderers[i]->SetActive(false); // 세팅해두고 일단 렌더를 끈다. AddDoor 함수에서 호출되는 RoomDir 방향의 문만 렌더한다.
	}

	DoorRenderers[static_cast<int>(RoomDir::LEFT) - 1]->CreateAnimation("Door_Left_Open"    , "NormalRoomDoor.png", 0, 0, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::LEFT) - 1]->CreateAnimation("Door_Left_Lock"    , "NormalRoomDoor.png", 4, 4, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::LEFT) - 1]->CreateAnimation("Door_Left_LockAnim", "NormalDoor"        , 12, 15, 0.15f, false);
	DoorRenderers[static_cast<int>(RoomDir::LEFT) - 1]->CreateAnimation("Door_Left_OpenAnim", "OpenDoor"          , 12, 15, 0.15f, false);

	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->CreateAnimation("Door_Right_Open"    , "NormalRoomDoor.png", 1, 1, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->CreateAnimation("Door_Right_Lock"    , "NormalRoomDoor.png", 5, 5, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->CreateAnimation("Door_Right_LockAnim", "NormalDoor"        , 4, 7, 0.15f, false);
	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->CreateAnimation("Door_Right_OpenAnim", "OpenDoor"          , 4, 7, 0.15f, false);

	DoorRenderers[static_cast<int>(RoomDir::UP) - 1]->CreateAnimation("Door_Up_Open", "NormalRoomDoor.png", 2, 2, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::UP) - 1]->CreateAnimation("Door_Up_Lock", "NormalRoomDoor.png", 6, 6, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::UP) - 1]->CreateAnimation("Door_Up_LockAnim", "NormalDoor", 0, 3, 0.15f, false);
	DoorRenderers[static_cast<int>(RoomDir::UP) - 1]->CreateAnimation("Door_Up_OpenAnim", "OpenDoor", 0, 3, 0.15f, false);

	DoorRenderers[static_cast<int>(RoomDir::DOWN) - 1]->CreateAnimation("Door_Down_Open", "NormalRoomDoor.png", 3, 3, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::DOWN) - 1]->CreateAnimation("Door_Down_Lock", "NormalRoomDoor.png", 7, 7, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::DOWN) - 1]->CreateAnimation("Door_Down_LockAnim", "NormalDoor", 8, 11, 0.15f, false);
	DoorRenderers[static_cast<int>(RoomDir::DOWN) - 1]->CreateAnimation("Door_Down_OpenAnim", "OpenDoor", 8, 11, 0.15f, false);

	DoorRenderers[static_cast<int>(RoomDir::LEFT)  - 1]->ChangeAnimation("Door_Left_Open");
	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->ChangeAnimation("Door_Right_Open");
	DoorRenderers[static_cast<int>(RoomDir::UP)    - 1]->ChangeAnimation("Door_Up_Open");
	DoorRenderers[static_cast<int>(RoomDir::DOWN)  - 1]->ChangeAnimation("Door_Down_Open");

}

ARoom::~ARoom()
{
	// map에서 insert한 자료들을 모두 map에서 delete하기 때문에 따로 소멸자를 호출해줄 필요가 없다.
}


