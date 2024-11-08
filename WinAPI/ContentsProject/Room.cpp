#include "PreCompile.h"
#include "Room.h"

#include <EngineBase/EngineDebug.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"
#include "PlayGameMode.h"

ARoom::ARoom()
{
	SetActorLocation({ Global::WindowSize.Half().iX(), Global::WindowSize.Half().iY()});
	SetActorScale(Global::WindowSize);

	float ScaleX = Global::WindowSize.X / 960;
	float ScaleY = Global::WindowSize.Y / 540;
	RoomScale = { 848.0f * ScaleX * GetActorScale().X / Global::WindowSize.X , 536.0f * ScaleY * GetActorScale().Y / Global::WindowSize.Y };

	RoomRenderer = CreateDefaultSubObject<USpriteRenderer>();
	RoomRenderer->SetSprite("SampleMap(848,536).png");
	RoomRenderer->SetComponentScale(RoomScale);
	RoomRenderer->SetOrder(ERenderOrder::BACKGROUND);

	BolderLineRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BolderLineRenderer->SetSprite("BolderLine.png");
	BolderLineRenderer->SetComponentScale(GetActorScale());
	BolderLineRenderer->SetComponentLocation({ RoomRenderer->GetComponentLocation().X, RoomRenderer->GetComponentLocation().Y});
	BolderLineRenderer->SetOrder(ERenderOrder::BOLDERLINE);

	DoorSpriteSetting();	

	DebugOn();
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
	FVector2D CurLocation = this->GetActorLocation();
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
	case RoomDir::NONE:
		break;
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
	case RoomDir::MAX:
		break;
	default:
		break;
	}

    return nullptr;
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
		//DoorRenderers[i]->SetComponentScale({ 250, 240 });
		DoorRenderers[i]->SetComponentScale({ 128, 128 });
		DoorRenderers[i]->SetOrder(ERenderOrder::DOOR);
	}

	DoorRenderers[static_cast<int>(RoomDir::LEFT) - 1]->CreateAnimation("Door_Left_Open", "NormalRoomDoor.png", 0, 0, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::LEFT) - 1]->CreateAnimation("Door_Left_Lock", "NormalRoomDoor.png", 4, 4, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::LEFT) - 1]->CreateAnimation("Door_Left_LockAnim", "NormalDoor", 12, 15, 0.15f, false);

	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->CreateAnimation("Door_Right_Open", "NormalRoomDoor.png", 1, 1, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->CreateAnimation("Door_Right_Lock", "NormalRoomDoor.png", 5, 5, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->CreateAnimation("Door_Right_LockAnim", "NormalDoor", 4, 7, 0.15f, false);
	
	DoorRenderers[static_cast<int>(RoomDir::UP) - 1]->CreateAnimation("Door_Up_Open", "NormalRoomDoor.png", 2, 2, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::UP) - 1]->CreateAnimation("Door_Up_Lock", "NormalRoomDoor.png", 6, 6, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::UP) - 1]->CreateAnimation("Door_Up_LockAnim", "NormalDoor", 0, 3, 0.15f, false);

	DoorRenderers[static_cast<int>(RoomDir::DOWN) - 1]->CreateAnimation("Door_Down_Open", "NormalRoomDoor.png", 3, 3, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::DOWN) - 1]->CreateAnimation("Door_Down_Lock", "NormalRoomDoor.png", 7, 7, 0.1f, false);
	DoorRenderers[static_cast<int>(RoomDir::DOWN) - 1]->CreateAnimation("Door_Down_LockAnim", "NormalDoor", 8, 11, 0.15f, false);

	DoorRenderers[static_cast<int>(RoomDir::LEFT)  - 1]->ChangeAnimation("Door_Left_Open" );
	DoorRenderers[static_cast<int>(RoomDir::RIGHT) - 1]->ChangeAnimation("Door_Right_Open");
	DoorRenderers[static_cast<int>(RoomDir::UP)    - 1]->ChangeAnimation("Door_Up_Open"   );
	DoorRenderers[static_cast<int>(RoomDir::DOWN)  - 1]->ChangeAnimation("Door_Down_Open" );

	// 세팅해두고 일단 렌더를 끈다. AddDoor 함수에서 호출되는 RoomDir 방향의 문만 렌더한다.
	for (int i = 0; i < DoorRenderers.size(); i++)
	{
		DoorRenderers[i]->SetActive(false);
	}
}

void ARoom::AddDoor(RoomDir _Dir, ARoom* _ConnectedRoom)
{	
	FVector2D DoorPos = FVector2D::ZERO;
	//FVector2D RoomPos = _ConnectedRoom->GetActorLocation();
	FVector2D DoorOffestX = FVector2D(RoomScale.Half().iX(), 0);
	FVector2D DoorOffestY = FVector2D(0, RoomScale.Half().iY());
	FVector2D OffestX = { 55, 0 };
	FVector2D OffestY = { 0, 50};

	switch (_Dir)
	{
	case RoomDir::LEFT:
		DoorPos = -1 * DoorOffestX  + OffestX;
		//DoorRenderers[0]->ChangeAnimation("Door_Left_Open");
		DoorRenderers[0]->ChangeAnimation("Door_Left_LockAnim");
		DoorRenderers[0]->SetComponentLocation(DoorPos);
		DoorRenderers[0]->SetActive(true);
		break;
	case RoomDir::RIGHT:
		DoorPos = DoorOffestX - OffestX;
		//DoorRenderers[1]->ChangeAnimation("Door_Right_Open");
		DoorRenderers[1]->ChangeAnimation("Door_Right_LockAnim");
		DoorRenderers[1]->SetComponentLocation(DoorPos);
		DoorRenderers[1]->SetActive(true);
		break;
	case RoomDir::UP:
		DoorPos = -1 * DoorOffestY + OffestY;
		//DoorRenderers[2]->ChangeAnimation("Door_Up_Open");
		DoorRenderers[2]->ChangeAnimation("Door_Up_LockAnim");
		DoorRenderers[2]->SetComponentLocation(DoorPos);
		DoorRenderers[2]->SetActive(true);
		break;
	case RoomDir::DOWN:
		DoorPos = DoorOffestY - OffestY;
		//DoorRenderers[3]->ChangeAnimation("Door_Down_Open");
		DoorRenderers[3]->ChangeAnimation("Door_Down_LockAnim");
		DoorRenderers[3]->SetComponentLocation(DoorPos);
		DoorRenderers[3]->SetActive(true);
		break;
	default:
		break;
	}
}

ARoom::~ARoom()
{
	// map에서 insert한 자료들을 모두 map에서 delete하기 때문에 따로 소멸자를 호출해줄 필요가 없다.
}

void ARoom::BeginPlay()
{
	Super::BeginPlay();

	// BaseRoom에만 생성되도록 코드를 수정하여야 함.
	if ("BaseRoom" == GetName())
	{
		ControlsRenderer = CreateDefaultSubObject<USpriteRenderer>();
		ControlsRenderer->SetSprite("controls.png");
		ControlsRenderer->SetComponentLocation({ GetActorLocation().iX() - Global::WindowHalfScale.iX(), GetActorLocation().iY() - Global::WindowHalfScale.iY() - 20 });
		ControlsRenderer->SetComponentScale({ 655, 145 }); // 385, 85
		ControlsRenderer->SetOrder(ERenderOrder::CONTROLS);
	}
}

void ARoom::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}





