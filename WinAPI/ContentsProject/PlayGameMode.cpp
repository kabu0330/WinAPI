#include "PreCompile.h"
#include "PlayGameMode.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/EngineCoreDebug.h>
#include <EnginePlatform/EngineInput.h>

#include "Global.h"
#include "ContentsCore.h"
#include "Room.h"
#include "Player.h"
#include "PlaySceneUI.h"
#include "ContentsEnum.h"

ARoom* APlayGameMode::CurRoom = nullptr;

APlayGameMode::APlayGameMode()
{
}

APlayGameMode::~APlayGameMode()
{
}

void APlayGameMode::BeginPlay()
{
	// 레벨이 만들어지면 최초 1회 세팅은 여기서 한다.
	// 레벨 마다 만들어줘야 한다. 
	// ATitleGameMode, EndingGameMode, BossIntroGameMode
	// 레벨의 생성과 동시에 만들어져야 하는 것들은 여기서 만든다.
	// ex. 배경
	
	// Player의 BeginPlay 함수에서도 만들어도 동일한 효과를 볼 수 있지만
	// Player로서의 순수한 독립성을 지켜주기 위해서 Player의 기능이 아니면 모두 여기서 만든다.
	GetWorld()->SetCameraPos({0, 0});
	//GetWorld()->SetCameraPos(GetWorld()->GetPawn()->GetActorLocation());
	GetWorld()->SetCameraToMainPawn(false);


	ARoom* BaseRoom    = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom0 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom1 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom2 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom3 = GetWorld()->SpawnActor<ARoom>();
	ARoom* BossRoom    = GetWorld()->SpawnActor<ARoom>();

	   BaseRoom->InterLinkRoom(MinionRoom0, RoomDir::LEFT );
	   BaseRoom->InterLinkRoom(MinionRoom1, RoomDir::RIGHT);
	   BaseRoom->InterLinkRoom(MinionRoom2, RoomDir::UP   );
	   BaseRoom->InterLinkRoom(MinionRoom3, RoomDir::DOWN );
	MinionRoom3->InterLinkRoom(BossRoom   , RoomDir::DOWN );

	   BaseRoom->SetName("BaseRoom"   );
	MinionRoom0->SetName("MinionRoom0");
	MinionRoom1->SetName("MinionRoom1");
	MinionRoom2->SetName("MinionRoom2");
	MinionRoom3->SetName("MinionRoom3");
	   BossRoom->SetName("BossRoom"   );
	
	// 플레이어가 현재 속한 방 위치 구하기 위한 코드 중 일부
	CurRoom = BaseRoom;

	AllRooms.push_back(BaseRoom);
	AllRooms.push_back(MinionRoom0);
	AllRooms.push_back(MinionRoom1);
	AllRooms.push_back(MinionRoom2);
	AllRooms.push_back(MinionRoom3);
	AllRooms.push_back(BossRoom);

	UISetting();
}

void APlayGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	PlayerCurRoom();


	
	EngineDebug(_DeltaTime);
}

void APlayGameMode::UISetting()
{
	APlaySceneUI* PickUpNumber = GetWorld()->SpawnActor<APlaySceneUI>();
	PickUpNumber->SetTextSpriteName("pickup.png");
	//PickUpNumber->SetTextSpriteName("banner.png");
	PickUpNumber->SetOrder(ERenderOrder::UI);
	PickUpNumber->SetTextScale({ 18, 21 }); // 10, 12
	PickUpNumber->SetActorLocation({ 200, 200 });
	PickUpNumber->SetValue(2);
}

// 일단 보류
void APlayGameMode::PlayerCurRoom()
{
	FVector2D PlayerPos = GetWorld()->GetPawn()->GetActorLocation();
	FVector2D RoomSize = CurRoom->GetActorScale();
	PlayerPos = PlayerPos / RoomSize;
	

	for (int i = 0; i < AllRooms.size(); i++)
	{
		FVector2D RoomPos = AllRooms[i]->GetActorLocation() / RoomSize;

		int a = 0;
		if (true)
		{
			std::string CurRoomName = AllRooms[i]->GetName();
			CurRoom->SetName(CurRoomName);

			int a = 0;
		}


	}
}

void APlayGameMode::EngineDebug(float _DeltaTime)
{
	FVector2D MousePos = UEngineAPICore::GetCore()->GetMainWindow().GetMousePos();
	UEngineDebug::CoreOutPutString("FPS : " + std::to_string(static_cast<int>(1.0f / _DeltaTime)));
	UEngineDebug::CoreOutPutString("Player Pos : " + GetWorld()->GetPawn()->GetActorLocation().ToString());
	UEngineDebug::CoreOutPutString("Camera Pos : " + GetWorld()->GetCameraPos().ToString());
	//UEngineDebug::CoreOutPutString("Mouse Pos : " + MousePos.ToString());
	UEngineDebug::CoreOutPutString("Room : " + APlayGameMode::GetCurRoom()->GetName());

	if (true == UEngineInput::GetInst().IsDown('B'))
	{
		UEngineDebug::SwitchIsDebug();
	}
}




