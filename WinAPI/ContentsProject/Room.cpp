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
#include "AttackFly.h"
#include "Fade.h"

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

void ARoom::BeginPlay()
{
	Super::BeginPlay();

	Player = dynamic_cast<APlayer*>(GetWorld()->GetPawn());

	// 맵 이름에 따라 이미지를 바꾼다.
	switch (RoomType)
	{
	case ERoomType::BASE:
	{
		RoomRenderer->SetSprite("Room_01.png");
		ControlsRenderer = CreateDefaultSubObject<USpriteRenderer>();
		ControlsRenderer->SetSprite("controls.png");
		ControlsRenderer->SetComponentLocation({ GetActorLocation().iX() - Global::WindowHalfScale.iX(), GetActorLocation().iY() - Global::WindowHalfScale.iY() - 60 });
		ControlsRenderer->SetComponentScale({ 655, 145 }); // 385, 85
		ControlsRenderer->SetOrder(ERenderOrder::Controls);

		FVector2D ControlsPos = ControlsRenderer->GetComponentLocation();
		ButtonsRenderer[0]->SetComponentLocation({ ControlsPos.X - 293.0f, ControlsPos.Y + 75.0f });
		FVector2D ButtonPos = ButtonsRenderer[0]->GetComponentLocation();
		float Gap = 30.0f;

		for (int i = 1; i < 4; i++)
		{
			ButtonsRenderer[i]->SetComponentLocation({ButtonPos.X + (Gap * static_cast<float>(i)), ControlsPos.Y + 75.0f });
		}

		for (int i = 4; i < 8; i++)
		{
			ButtonsRenderer[i]->SetComponentLocation({ ControlsPos.X - 124.0f + (Gap * static_cast<float>(i - 4)), ControlsPos.Y + 75.0f });
		}

		ButtonsRenderer[8]->SetComponentLocation({ ControlsPos.X + 90.0f, ControlsPos.Y + 75.0f });
		break;
	}
	case ERoomType::TREASURE:
		RoomRenderer->SetSprite("Room_01.png");
		break;
	case ERoomType::BOSS:
		RoomRenderer->SetSprite("Room_03.png");
		break;
	case ERoomType::NORMAL:
	case ERoomType::NONE:
	default:
		RoomRenderer->SetSprite("Room_02.png");
		break;
	}
}

void ARoom::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (this != CurRoom)
	{
		return;
	}

	// Player Warp
	WarpCollisionCheck(_DeltaTime);
	Warp(_DeltaTime);
	
	OpenTheDoor();
	CloseTheDoor();
	
	HasInitMonster();


	Sounds();
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
			// 플레이어 정지
			Player->SetMovement(false);
			Player->SetDir(FVector2D::ZERO);

			// 문 충돌체 임시 제거
			IsWarp = true;
			TemporarilyHideDoorCollision(); 

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

	APlayer* DynamicPlayer = dynamic_cast<APlayer*>(Player);
	DynamicPlayer->SetDir(FVector2D::ZERO);

	// 플레이어 위치 이동
	WarpPlayerSetting(); 

	float CameraLerpTime = 0.1f;
	LerpAlpha += _DeltaTime / CameraLerpTime;
	LerpAlpha = FVector2D::Clamp(CameraMoveTime / CameraLerpTime, 0.0f, 1.0f);

	FVector2D CamPos = FVector2D::Lerp(StartCameraPos, EndCameraPos, LerpAlpha);
	GetWorld()->SetCameraPos(CamPos);

	CameraMoveTime += _DeltaTime;
	if (LerpAlpha >= 1.0f)
	{
		CameraMoveDir = FVector2D::ZERO;
		CurRoom = Rooms[MoveDir];

		CameraMove = false;
		CameraMoveTime = 0.0f;
		TimeEventer.PushEvent(0.0f, [this]() { Player->SetMovement(false); });
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
		Offset = -FVector2D(0, GetActorScale().Half().iY() + 120);
		break;
	case RoomDir::DOWN:
		Offset = FVector2D(0, GetActorScale().Half().iY() + 120);
		break;
	default:
		break;
	}

	TargetPlayerPos = GetActorLocation() + Offset;
	Player->SetActorLocation(TargetPlayerPos);

	TimeEventer.PushEvent(1.5f, [this]() {IsWarp = false; });

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
	FVector2D OffestY = { 0, 64};
	DoorCollisionScale = FVector2D(60, 60);

	float DoorEffectOffest = 85.0f;

	switch (_Dir)
	{
	case RoomDir::LEFT:
		DoorPos = -1 * DoorOffestX  + OffestX;
		DoorRenderers[0]->ChangeAnimation("Door_Left_OpenAnim");
		DoorRenderers[0]->SetComponentLocation(DoorPos);
		DoorRenderers[0]->SetActive(true);

		DoorCollisions[0]->SetComponentScale(DoorCollisionScale);
		DoorCollisions[0]->SetActive(true);

		DoorRendererMap.insert({ _Dir , DoorRenderers[0] });
		DoorCollisionMap.insert({ _Dir ,DoorCollisions[0] });

		BossDoorOpenEffect->SetComponentLocation({ DoorPos.X + DoorEffectOffest, DoorPos.Y });
		BossDoorOpenEffect->ChangeAnimation("BossDoorEffect_Left");
		break;
	case RoomDir::RIGHT:
		DoorPos = DoorOffestX - OffestX;
		DoorRenderers[1]->ChangeAnimation("Door_Right_OpenAnim");
		DoorRenderers[1]->SetComponentLocation(DoorPos);
		DoorRenderers[1]->SetActive(true);

		DoorCollisions[1]->SetComponentScale(DoorCollisionScale);
		DoorCollisions[1]->SetActive(true);

		DoorRendererMap.insert({ _Dir , DoorRenderers[1] });
		DoorCollisionMap.insert({ _Dir ,DoorCollisions[1]});

		BossDoorOpenEffect->SetComponentLocation({ DoorPos.X - DoorEffectOffest, DoorPos.Y });
		BossDoorOpenEffect->ChangeAnimation("BossDoorEffect_Right");
		break;
	case RoomDir::UP:
		DoorPos = -1 * DoorOffestY + OffestY;
		DoorRenderers[2]->ChangeAnimation("Door_Up_OpenAnim");
		DoorRenderers[2]->SetComponentLocation(DoorPos);
		DoorRenderers[2]->SetActive(true);

		DoorCollisions[2]->SetComponentScale(DoorCollisionScale);
		DoorCollisions[2]->SetActive(true);
		DoorRendererMap.insert({ _Dir , DoorRenderers[2] });
		DoorCollisionMap.insert({ _Dir ,DoorCollisions[2] });

		BossDoorOpenEffect->SetComponentLocation({ DoorPos.X, DoorPos.Y + DoorEffectOffest });
		BossDoorOpenEffect->ChangeAnimation("BossDoorEffect_Up");
		break;
	case RoomDir::DOWN:
		DoorPos = DoorOffestY - OffestY;
		DoorRenderers[3]->ChangeAnimation("Door_Down_OpenAnim");
		DoorRenderers[3]->SetComponentLocation(DoorPos);
		DoorRenderers[3]->SetActive(true);

		DoorCollisions[3]->SetComponentScale(DoorCollisionScale);
		DoorCollisions[3]->SetActive(true);

		DoorRendererMap.insert({ _Dir , DoorRenderers[3] });
		DoorCollisionMap.insert({ _Dir ,DoorCollisions[3] });

		BossDoorOpenEffect->SetComponentLocation({ DoorPos.X, DoorPos.Y - DoorEffectOffest });
		BossDoorOpenEffect->ChangeAnimation("BossDoorEffect_Down");
		break;
	default:
		break;
	}

	for (int i = 0; i < DoorCollisions.size(); i++)
	{
		DoorCollisions[i]->SetComponentLocation(DoorRenderers[i]->GetComponentLocation());
	}
}

void ARoom::OpenTheDoor()
{
	if (true == IsWarp)
	{
		return;
	}
	if (0 != GetAliveMonsterCount())
	{
		return;
	}

	std::map<RoomDir, USpriteRenderer*>::iterator StartIter = DoorRendererMap.begin();
	std::map<RoomDir, USpriteRenderer*>::iterator EndIter = DoorRendererMap.end();

	for (;StartIter != EndIter; ++StartIter)
	{
		RoomDir Dir = StartIter->first;
		USpriteRenderer* Door = StartIter->second;

		if (nullptr == Door)
		{
			continue;
		}

		std::string DirString = SwitchEnumToString(Dir);
		std::string MyName = GetName();
		std::string FindRoomName = "BossRoom";
		ERoomType Type = ERoomType::BOSS;
		if (true == IsNextRoom(Type, Dir) || Type == GetRoomType())
		{
			std::string AnimationName = "BossDoor_" + DirString + "_OpenAnim";
			Door->ChangeAnimation(AnimationName);
			BossDoorOpenEffect->SetComponentScale({ 96, 96 });
			continue;
		}

		FindRoomName = "TreasureRoom";
		Type = ERoomType::TREASURE;
		if (true == IsNextRoom(Type, Dir) || Type == GetRoomType())
		{
			std::string AnimationName = "Treasure_" + DirString + "_OpenAnim";
			Door->ChangeAnimation(AnimationName);
			continue;
		}

		std::string AnimationName = "Door_" + DirString + "_OpenAnim";
		Door->ChangeAnimation(AnimationName);
	}

	{
		std::map<RoomDir, U2DCollision*>::iterator StartIter = DoorCollisionMap.begin();
		std::map<RoomDir, U2DCollision*>::iterator EndIter = DoorCollisionMap.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			RoomDir Dir = StartIter->first;
			U2DCollision* Door = StartIter->second;

			if (nullptr != Door)
			{
				Door->SetComponentScale(DoorCollisionScale);
			}
		}
	}
}

void ARoom::CloseTheDoor()
{
	if (0 == GetAliveMonsterCount())
	{
		return;
	}

	std::map<RoomDir, USpriteRenderer*>::iterator StartIter = DoorRendererMap.begin();
	std::map<RoomDir, USpriteRenderer*>::iterator EndIter = DoorRendererMap.end();
	
	for (; StartIter != EndIter; ++StartIter)
	{
		RoomDir Dir = StartIter->first;
		USpriteRenderer* Door = StartIter->second;

		if (nullptr == Door)
		{
			continue;
		}

		std::string DirString = SwitchEnumToString(Dir);
		std::string MyName = GetName();
		std::string FindRoomName = "BossRoom";
		ERoomType Type = ERoomType::BOSS;
		BossDoorOpenEffect->SetComponentScale({ 0, 0});
		if (true == IsNextRoom(Type, Dir) || Type == GetRoomType())
		{
			std::string AnimationName = "BossDoor_" + DirString + "_CloseAnim";
			Door->ChangeAnimation(AnimationName);
			continue;
		}

		FindRoomName = "TreasureRoom";
		Type = ERoomType::TREASURE;
		if (true == IsNextRoom(Type, Dir) || Type == GetRoomType())
		{
			std::string AnimationName = "Treasure_" + DirString + "_CloseAnim";
			Door->ChangeAnimation(AnimationName);
			continue;
		}

		std::string AnimationName = "Door_" + DirString + "_CloseAnim";
		Door->ChangeAnimation(AnimationName);
	}

	{
		std::map<RoomDir, U2DCollision*>::iterator StartIter = DoorCollisionMap.begin();
		std::map<RoomDir, U2DCollision*>::iterator EndIter = DoorCollisionMap.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			RoomDir Dir = StartIter->first;
			U2DCollision* Door = StartIter->second;

			if (nullptr != Door)
			{
				Door->SetComponentScale({ 0, 0 });
			}
		}
	}
	
}

bool ARoom::IsNextRoom(ERoomType _RoomType, RoomDir _Dir)
{
	ERoomType Type = Rooms[_Dir]->GetRoomType();
	if (_RoomType == Type)
	{
		return true;
	}
	return false;
}

std::string ARoom::SwitchEnumToString(RoomDir _Dir)
{
	switch (_Dir)
	{
	case RoomDir::LEFT:
		return "Left";
	case RoomDir::RIGHT:
		return "Right";
	case RoomDir::UP:
		return "Up";
	case RoomDir::DOWN:
		return "Down";
	default:
		return "";
	}
}

void ARoom::TemporarilyHideDoorCollision()
{
	IsWarp = true;

	std::map<RoomDir, U2DCollision*>::iterator StartIter = DoorCollisionMap.begin();
	std::map<RoomDir, U2DCollision*>::iterator EndIter = DoorCollisionMap.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		RoomDir Dir = StartIter->first;
		U2DCollision* Door = StartIter->second;

		if (nullptr != Door)
		{
			Door->SetComponentScale({ 0, 0 });
		}
	}
}

void ARoom::SpriteSetting()
{
	RoomRenderer = CreateDefaultSubObject<USpriteRenderer>();
	RoomRenderer->SetSprite("Room_01.png");
	RoomRenderer->SetComponentScale(RoomScale);
	RoomRenderer->SetOrder(ERenderOrder::Background);

	for (int i = 0; i < 9; i++)
	{
		ButtonsRenderer[i] = CreateDefaultSubObject<USpriteRenderer>();
		ButtonsRenderer[i]->SetComponentScale(ButtonScale);
		ButtonsRenderer[i]->SetOrder(ERenderOrder::Controls);
	}

	ButtonsRenderer[0]->SetSprite("controls_buttons.png", 22); // W
	ButtonsRenderer[1]->SetSprite("controls_buttons.png", 0); // A
	ButtonsRenderer[2]->SetSprite("controls_buttons.png", 18); // S
	ButtonsRenderer[3]->SetSprite("controls_buttons.png", 3); // D
	ButtonsRenderer[4]->SetSprite("controls_buttons.png", 69); // 좌
	ButtonsRenderer[5]->SetSprite("controls_buttons.png", 70); // 우
	ButtonsRenderer[6]->SetSprite("controls_buttons.png", 71); // 아래
	ButtonsRenderer[7]->SetSprite("controls_buttons.png", 72); // 위
	ButtonsRenderer[8]->SetSprite("controls_buttons.png", 4); // E

}

void ARoom::CollisionSetting()
{
	RoomSizeOffsetX = -300.0f;
	RoomSizeOffsetY = -190.0f;

	RoomCollision = CreateDefaultSubObject<U2DCollision>();
	RoomCollision->SetComponentLocation({ 0, 0 });
	RoomCollision->SetComponentScale({ RoomScale.X + RoomSizeOffsetX, RoomScale.Y + RoomSizeOffsetY });
	RoomCollision->SetCollisionGroup(ECollisionGroup::Room_Wall);
	RoomCollision->SetCollisionType(ECollisionType::Rect);

	float OffsetX = -200.0f;
	float OffsetY = -100.0f;

	ClampTearCollision = CreateDefaultSubObject<U2DCollision>();
	ClampTearCollision->SetComponentLocation({ 0, 0 });
	ClampTearCollision->SetComponentScale({ RoomScale.X + OffsetX, RoomScale.Y + OffsetY });
	ClampTearCollision->SetCollisionGroup(ECollisionGroup::Room_TearBoundary);
	ClampTearCollision->SetCollisionType(ECollisionType::Rect);


	DoorCollisions.resize(4);
	for (int i = 0; i < DoorCollisions.size(); i++)
	{
		DoorCollisions[i] = CreateDefaultSubObject<U2DCollision>();
		DoorCollisions[i]->SetComponentScale({ 0, 0 }); // 여기서 크기를 결정하지 않는다.
		DoorCollisions[i]->SetCollisionGroup(ECollisionGroup::Warp);
		DoorCollisions[i]->SetCollisionType(ECollisionType::Rect);
		DoorCollisions[i]->SetActive(false);
	}
}

void ARoom::Sounds()
{
	if (this != CurRoom)
	{
		return;
	}
	if (false == IsTreasureRoomEnterance)
	{
		if (ERoomType::TREASURE == this->GetRoomType())
		{
			UEngineSound::AllSoundStop();
			Sound = UEngineSound::Play("treasure_room_discovera.ogg");

			TimeEventer.PushEvent(4.0f, []() {APlayGameMode::GetPlayGameModeBGM() = UEngineSound::Play("diptera_sonata_basement.ogg"); });
			
			IsTreasureRoomEnterance = true;
			return;
		}
	}
	if (true == IsFirstEnterance)
	{
		return;
	}

	// Close
	if (0 != GetAliveMonsterCount()) 
	{
		if (true == CloseDoorSoundPlay)
		{
			return;
		}
		Sound = UEngineSound::Play("door_heavy_close.wav");
		//Sound = UEngineSound::Play("summonsound.wav");
		CloseDoorSoundPlay = true;
		return;
	}
	// Open
	else if (0 == GetAliveMonsterCount())
	{
		if (true == OpenDoorSoundPlay)
		{
			return;
		}

		Sound = UEngineSound::Play("door_heavy_open.wav");
		OpenDoorSoundPlay = true;
		IsFirstEnterance = true;
		return;
	}

}

void ARoom::HasInitMonster()
{
	if (0 == GetAliveMonsterCount() && false == IsInitMonsterExist)
	{
		OpenDoorSoundPlay = true;
		IsInitMonsterExist = true;
	}
	else
	{
		IsInitMonsterExist = true;
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
		DoorRenderers[i]->SetOrder(ERenderOrder::Door);
		DoorRenderers[i]->SetActive(false); // 세팅해두고 일단 렌더를 끈다. AddDoor 함수에서 호출되는 RoomDir 방향의 문만 렌더한다.
	}

	float AnimationSpeed = 0.12f;

	// Normal Door
	DoorRenderers[static_cast<int>(RoomDir::LEFT) - 1]->CreateAnimation("Door_Left_Open"    , "NormalRoomDoor.png", 0, 0, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::LEFT) - 1]->CreateAnimation("Door_Left_CloseAnim", "Normal_CloseDoor"        , 12, 15, AnimationSpeed, false);
	DoorRenderers[static_cast<int>(RoomDir::LEFT) - 1]->CreateAnimation("Door_Left_OpenAnim", "Normal_OpenDoor"          , 12, 15, AnimationSpeed, false);

	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->CreateAnimation("Door_Right_Open"    , "NormalRoomDoor.png", 1, 1, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->CreateAnimation("Door_Right_CloseAnim", "Normal_CloseDoor"        , 4, 7, AnimationSpeed, false);
	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->CreateAnimation("Door_Right_OpenAnim", "Normal_OpenDoor"          , 4, 7, AnimationSpeed, false);

	DoorRenderers[static_cast<int>(RoomDir::UP) - 1]->CreateAnimation("Door_Up_Open", "NormalRoomDoor.png", 2, 2, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::UP) - 1]->CreateAnimation("Door_Up_CloseAnim", "Normal_CloseDoor", 0, 3, AnimationSpeed, false);
	DoorRenderers[static_cast<int>(RoomDir::UP) - 1]->CreateAnimation("Door_Up_OpenAnim", "Normal_OpenDoor", 0, 3, AnimationSpeed, false);

	DoorRenderers[static_cast<int>(RoomDir::DOWN) - 1]->CreateAnimation("Door_Down_Open", "NormalRoomDoor.png", 3, 3, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::DOWN) - 1]->CreateAnimation("Door_Down_CloseAnim", "Normal_CloseDoor", 8, 11, AnimationSpeed, false);
	DoorRenderers[static_cast<int>(RoomDir::DOWN) - 1]->CreateAnimation("Door_Down_OpenAnim", "Normal_OpenDoor", 8, 11, AnimationSpeed, false);

	//////////////////////////////////////////////////
	// Boss Door

	DoorRenderers[static_cast<int>(RoomDir::LEFT) - 1]->CreateAnimation("BossDoor_Left_Open", "Boss_CloseDoor", 4, 4, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::LEFT) - 1]->CreateAnimation("BossDoor_Left_CloseAnim", "Boss_CloseDoor", 12, 15, AnimationSpeed, false);
	DoorRenderers[static_cast<int>(RoomDir::LEFT) - 1]->CreateAnimation("BossDoor_Left_OpenAnim", "Boss_OpenDoor", 12, 15, AnimationSpeed, false);

	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->CreateAnimation("BossDoor_Right_Open", "Boss_CloseDoor", 12, 12, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->CreateAnimation("BossDoor_Right_CloseAnim", "Boss_CloseDoor", 4, 7, AnimationSpeed, false);
	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->CreateAnimation("BossDoor_Right_OpenAnim", "Boss_OpenDoor", 4, 7, AnimationSpeed, false);

	DoorRenderers[static_cast<int>(RoomDir::UP) - 1]->CreateAnimation("BossDoor_Up_Open", "Boss_CloseDoor", 0, 0, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::UP) - 1]->CreateAnimation("BossDoor_Up_CloseAnim", "Boss_CloseDoor", 0, 3, AnimationSpeed, false);
	DoorRenderers[static_cast<int>(RoomDir::UP) - 1]->CreateAnimation("BossDoor_Up_OpenAnim", "Boss_OpenDoor", 0, 3, AnimationSpeed, false);

	DoorRenderers[static_cast<int>(RoomDir::DOWN) - 1]->CreateAnimation("BossDoor_Down_Open", "Boss_CloseDoor", 4, 4, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::DOWN) - 1]->CreateAnimation("BossDoor_Down_CloseAnim", "Boss_CloseDoor", 8, 11, AnimationSpeed, false);
	DoorRenderers[static_cast<int>(RoomDir::DOWN) - 1]->CreateAnimation("BossDoor_Down_OpenAnim", "Boss_OpenDoor", 8, 11, AnimationSpeed, false);


	BossDoorOpenEffect = CreateDefaultSubObject<USpriteRenderer>();
	BossDoorOpenEffect->CreateAnimation("BossDoorEffect_Left" , "BossDoorEffect000.png", 0, 0, 0.1f, false);
	BossDoorOpenEffect->CreateAnimation("BossDoorEffect_Right", "BossDoorEffect001.png", 0, 0, 0.1f, false);
	BossDoorOpenEffect->CreateAnimation("BossDoorEffect_Up"   , "BossDoorEffect002.png", 0, 0, 0.1f, false);
	BossDoorOpenEffect->CreateAnimation("BossDoorEffect_Down" , "BossDoorEffect003.png", 0, 0, 0.1f, false);
	BossDoorOpenEffect->SetComponentScale({ 0, 0 });
	BossDoorOpenEffect->SetAlphaFloat(0.15f);
	BossDoorOpenEffect->SetOrder(ERenderOrder::Door);
	BossDoorOpenEffect->ChangeAnimation("BossDoorEffect_Left");

	//////////////////////////////////////////////////
	// Treasure Door

	DoorRenderers[static_cast<int>(RoomDir::LEFT) - 1]->CreateAnimation("Treasure_Left_Open", "Treasure_CloseDoor", 4, 4, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::LEFT) - 1]->CreateAnimation("Treasure_Left_CloseAnim", "Treasure_CloseDoor", 12, 15, AnimationSpeed, false);
	DoorRenderers[static_cast<int>(RoomDir::LEFT) - 1]->CreateAnimation("Treasure_Left_OpenAnim", "Treasure_OpenDoor", 12, 15, AnimationSpeed, false);

	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->CreateAnimation("Treasure_Right_Open", "Treasure_CloseDoor", 12, 12, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->CreateAnimation("Treasure_Right_CloseAnim", "Treasure_CloseDoor", 4, 7, AnimationSpeed, false);
	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->CreateAnimation("Treasure_Right_OpenAnim", "Treasure_OpenDoor", 4, 7, AnimationSpeed, false);

	DoorRenderers[static_cast<int>(RoomDir::UP) - 1]->CreateAnimation("Treasure_Up_Open", "Treasure_CloseDoor", 0, 0, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::UP) - 1]->CreateAnimation("Treasure_Up_CloseAnim", "Treasure_CloseDoor", 0, 3, AnimationSpeed, false);
	DoorRenderers[static_cast<int>(RoomDir::UP) - 1]->CreateAnimation("Treasure_Up_OpenAnim", "Treasure_OpenDoor", 0, 3, AnimationSpeed, false);

	DoorRenderers[static_cast<int>(RoomDir::DOWN) - 1]->CreateAnimation("Treasure_Down_Open", "Treasure_CloseDoor", 4, 4, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::DOWN) - 1]->CreateAnimation("Treasure_Down_CloseAnim", "Treasure_CloseDoor", 8, 11, AnimationSpeed, false);
	DoorRenderers[static_cast<int>(RoomDir::DOWN) - 1]->CreateAnimation("Treasure_Down_OpenAnim", "Treasure_OpenDoor", 8, 11, AnimationSpeed, false);



	DoorRenderers[static_cast<int>(RoomDir::LEFT)  - 1]->ChangeAnimation("Door_Left_Open");
	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->ChangeAnimation("Door_Right_Open");
	DoorRenderers[static_cast<int>(RoomDir::UP)    - 1]->ChangeAnimation("Door_Up_Open");
	DoorRenderers[static_cast<int>(RoomDir::DOWN)  - 1]->ChangeAnimation("Door_Down_Open");

}

int ARoom::GetAliveMonsterCount()
{
	if (CurRoom != this)
	{
		return 0;
	}
	
	int Count = 0;

	std::list<AMonster*>::iterator StartIter = Monsters.begin();
	std::list<AMonster*>::iterator EndIter = Monsters.end();
	for (; StartIter != EndIter; ++StartIter)
	{
		AMonster* Monster = *StartIter;
		if (nullptr != Monster)
		{
			++Count;
		}
	}

	return Count;
}

int ARoom::CountFly()
{	
	int FlyCount = 0;

	std::list<AMonster*>::iterator StartIter = Monsters.begin();
	std::list<AMonster*>::iterator EndIter = Monsters.end();
	for (; StartIter != EndIter; ++StartIter)
	{
		AAttackFly* Monster = dynamic_cast<AAttackFly*>(*StartIter);
		if (nullptr != Monster)
		{
			++FlyCount;
		}
	}
	return FlyCount;
}

ARoom::~ARoom()
{
	// map에서 insert한 자료들을 모두 map에서 delete하기 때문에 따로 소멸자를 호출해줄 필요가 없다.
}


