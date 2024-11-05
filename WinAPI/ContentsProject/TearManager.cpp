#include "PreCompile.h"
#include "TearManager.h"

ATearManager::ATearManager()
{
	// �� ���� �ڽ��� ���� ���尡 ����� �𸥴�.
}

ATearManager::~ATearManager()
{
	// SpawnActor�� ���� �����Ҵ��� �� �̻�, �޸� ���� ������ ATearManager���� ����.
	// ULevel�� ��� ����� �̾��ߴ�.
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


