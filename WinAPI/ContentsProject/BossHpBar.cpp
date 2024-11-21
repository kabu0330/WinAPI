#include "PreCompile.h"
#include "BossHpBar.h"
#include <EngineCore/SpriteRenderer.h>
#include "Monster.h"
#include "TheDukeOfFlies.h"
#include "ContentsEnum.h"
#include "Global.h"

ABossHpBar::ABossHpBar()
{
	SetName("BossHpBar");

	FVector2D Pos = { Global::WindowSize.X * 0.5f, Global::WindowSize.Y * 0.11f };
	Scale = { 330, 150 };
	BackRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BackRenderer->SetSprite("emptybar.png");
	BackRenderer->SetComponentLocation(Pos);
	BackRenderer->SetComponentScale(Scale);
	BackRenderer->SetOrder(ERenderOrder::UI_Back);
	BackRenderer->SetActive(true);
	BackRenderer->SetCameraEffect(false);

	FrontRenderer = CreateDefaultSubObject<USpriteRenderer>();
	FrontRenderer->SetSprite("fullbar.png");
	FrontRenderer->SetComponentLocation(Pos);
	FrontRenderer->SetPivotValue({ 0.0f, 0.5f });
	FrontRenderer->SetOrder(ERenderOrder::UI_Back);
	FrontRenderer->SetActive(true);
	FrontRenderer->SetCameraEffect(false);
	
}

void ABossHpBar::SetHpBar(AMonster* _Monster)
{
	Monster = _Monster;
	HpMax = Monster->GetHp();
	IsGetHp = true;
}

void ABossHpBar::BeginPlay()
{
	Super::BeginPlay();
}

void ABossHpBar::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	Update();
}

void ABossHpBar::Update()
{
	if (false == IsGetHp)
	{
		return;
	}
	int CurHp = Monster->GetHp();
	HpRatio = static_cast<float>(CurHp) / static_cast<float>(HpMax);
	FrontRenderer->SetComponentScale({ Scale.X * HpRatio, Scale.Y });
}

ABossHpBar::~ABossHpBar()
{
}