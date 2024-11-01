#include "PreCompile.h"
#include "PlayGameMode.h"

#include <EngineCore/EngineAPICore.h>

#include "Global.h"
#include "Room.h"
#include "Door.h"
#include "Player.h"

ARoom* APlayGameMode::CurRoom = nullptr;

APlayGameMode::APlayGameMode()
{
}

APlayGameMode::~APlayGameMode()
{
}

void APlayGameMode::BeginPlay()
{
	// ������ ��������� ���� 1ȸ ������ ���⼭ �Ѵ�.
	// ���� ���� �������� �Ѵ�. 
	// ATitleGameMode, EndingGameMode, BossIntroGameMode
	// ������ ������ ���ÿ� ��������� �ϴ� �͵��� ���⼭ �����.
	// ex. ���
	
	// Player�� BeginPlay �Լ������� ���� ������ ȿ���� �� �� ������
	// Player�μ��� ������ �������� �����ֱ� ���ؼ� Player�� ����� �ƴϸ� ��� ���⼭ �����.
	GetWorld()->SetCameraPos({0, 0});
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

	BaseRoom->SetName("BaseRoom");
	MinionRoom0->SetName("MinionRoom0");
	MinionRoom1->SetName("MinionRoom1");
	MinionRoom2->SetName("MinionRoom2");
	MinionRoom3->SetName("MinionRoom3");
	BossRoom->SetName("BossRoom");

	//ADoor* Door = GetWorld()->SpawnActor<ADoor>();
	CurRoom = BaseRoom;

	int a = 0;
}

void APlayGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

