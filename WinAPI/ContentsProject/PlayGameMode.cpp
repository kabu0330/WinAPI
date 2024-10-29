#include "PreCompile.h"
#include "PlayGameMode.h"
#include "Room.h"
#include <EngineCore/EngineAPICore.h>

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
	
	ARoom* BaseRoom    = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom0 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom1 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom2 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom3 = GetWorld()->SpawnActor<ARoom>();
	ARoom* BossRoom    = GetWorld()->SpawnActor<ARoom>();

	   BaseRoom->LinkRoom(MinionRoom0, RoomDir::LEFT );
	   BaseRoom->LinkRoom(MinionRoom1, RoomDir::RIGHT);
	   BaseRoom->LinkRoom(MinionRoom2, RoomDir::UP   );
	   BaseRoom->LinkRoom(MinionRoom3, RoomDir::DOWN );
	MinionRoom3->LinkRoom(BossRoom   , RoomDir::DOWN );

	int a = 0;
}

