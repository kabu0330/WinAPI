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
	SetActorScale(Global::WindowScale  );

	float ScaleX = Global::WindowSize.X / 960;
	float ScaleY = Global::WindowSize.Y / 540;
	RoomScale = { 848.0f * ScaleX * GetActorScale().X / Global::WindowScale.X , 536.0f * ScaleY * GetActorScale().Y / Global::WindowScale.Y };

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

	// BaseRoom에만 생성되도록 코드를 수정하여야 함.
	//ControlsRenderer = CreateDefaultSubObject<USpriteRenderer>();
	//ControlsRenderer->SetSprite("controls.png");
	//ControlsRenderer->SetComponentLocation(GetActorLocation());
	//ControlsRenderer->SetComponentScale({ 325, 85 });
	//ControlsRenderer->SetOrder(ERenderOrder::CONTROLS);
	
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
	DoorRenderer = CreateDefaultSubObject<USpriteRenderer>();
	//DoorRenderer->GetActorTransform().Location;
	DoorRenderer->SetComponentScale({ 250, 200 });
	DoorRenderer->SetOrder(ERenderOrder::DOOR);

	DoorRenderer->CreateAnimation("Door_Left_Open", "NormalRoomDoor.png", 0, 0, 0.1f, false);
	DoorRenderer->CreateAnimation("Door_Left_Lock", "NormalRoomDoor.png", 4, 4, 0.1f, false);

	DoorRenderer->CreateAnimation("Door_Right_Open", "NormalRoomDoor.png", 1, 1, 0.1f, false);
	DoorRenderer->CreateAnimation("Door_Right_Lock", "NormalRoomDoor.png", 5, 5, 0.1f, false);

	DoorRenderer->CreateAnimation("Door_Up_Open", "NormalRoomDoor.png", 2, 2, 0.1f, false);
	DoorRenderer->CreateAnimation("Door_Up_Lock", "NormalRoomDoor.png", 6, 6, 0.1f, false);

	DoorRenderer->CreateAnimation("Door_Down_Open", "NormalRoomDoor.png", 3, 3, 0.1f, false);
	DoorRenderer->CreateAnimation("Door_Down_Lock", "NormalRoomDoor.png", 7, 7, 0.1f, false);

	DoorRenderer->ChangeAnimation("Door_Left_Open");
	//DoorRenderer->ChangeAnimation("Door_Down_Open");
}

void ARoom::AddDoor(RoomDir _Dir, ARoom* _ConnectedRoom)
{
	FVector2D DoorPos = FVector2D::ZERO;
	FVector2D RoomPos = _ConnectedRoom->GetActorLocation();
	FVector2D DoorOffestX = FVector2D(RoomScale.Half().iX(), 0);
	FVector2D DoorOffestY = FVector2D(0, RoomScale.Half().iY());
	FVector2D OffestX = { 50, 0 };
	FVector2D OffestY = { 0, 50 };

	switch (_Dir)
	{
	case RoomDir::LEFT:
		DoorPos = -1 * DoorOffestX  + OffestX;
		DoorRenderer->ChangeAnimation("Door_Left_Open");
		break;
	case RoomDir::RIGHT:
		DoorPos = DoorOffestX - OffestX;
		DoorRenderer->ChangeAnimation("Door_Right_Open");
		break;
	case RoomDir::UP:
		DoorPos = -1 * DoorOffestY + OffestY;
		DoorRenderer->ChangeAnimation("Door_Up_Open");
		break;
	case RoomDir::DOWN:
		DoorPos = DoorOffestY - OffestY;
		DoorRenderer->ChangeAnimation("Door_Down_Open");
		break;
	default:
		break;
	}
	this;
	FVector2D Result = RoomPos + DoorPos;
	DoorRenderer->SetComponentLocation(DoorPos);
	DoorRenderers.push_back(DoorRenderer);

	int a = 0;

}



ARoom::~ARoom()
{
	// map에서 insert한 자료들을 모두 map에서 delete하기 때문에 따로 소멸자를 호출해줄 필요가 없다.
}

void ARoom::BeginPlay()
{
	Super::BeginPlay();

}

void ARoom::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}




