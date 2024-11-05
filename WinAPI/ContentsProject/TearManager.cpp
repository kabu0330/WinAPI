#include "PreCompile.h"
#include "TearManager.h"

ATearManager::ATearManager()
{
	// 이 때는 자신이 속한 월드가 어딘지 모른다.
}

ATearManager::~ATearManager()
{
	// SpawnActor를 통해 동적할당을 한 이상, 메모리 해제 권한은 ATearManager에겐 없다.
	// ULevel로 모든 목숨을 이양했다.
}

void ATearManager::BeginPlay()
{
	Super::BeginPlay();

	//ULevel* Ptr = GetWorld();
	Tears.resize(poolCount);
	Tears.clear();
	for (int i = 0; i < poolCount; i++)
	{
		ATear* Tear = GetWorld()->SpawnActor<ATear>();
		Tears.push_back(Tear);
	}
	int a = 0;
}

void ATearManager::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	for (int i = 0; i < poolCount; i++)
	{
		if (nullptr != Tears[i])
		{
			Tears[i]->Tick(_DeltaTime);
		}
	}
}

void ATearManager::Fire(FVector2D _StartPos, FVector2D _Dir)
{
	for (int i = 0; i < Tears.size(); i++)
	{
		if (true == Tears[i]->IsActive())
		{
			Tears[i]->Fire(_StartPos, _Dir);
			return;
		}
	}
}


