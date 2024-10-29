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
	// 레벨이 만들어지면 최초 1회 세팅은 여기서 한다.
	// 레벨 마다 만들어줘야 한다. 
	// ATitleGameMode, EndingGameMode, BossIntroGameMode
	// 레벨의 생성과 동시에 만들어져야 하는 것들은 여기서 만든다.
	// ex. 배경
	
	// Player의 BeginPlay 함수에서도 만들어도 동일한 효과를 볼 수 있지만
	// Player로서의 순수한 독립성을 지켜주기 위해서 Player의 기능이 아니면 모두 여기서 만든다.
	
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

	int a = 0;
}

