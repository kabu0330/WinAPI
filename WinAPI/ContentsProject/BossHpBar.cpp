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
	BackRenderer->SetActive(false);
	BackRenderer->SetCameraEffect(false);

	FrontRenderer = CreateDefaultSubObject<USpriteRenderer>();
	FrontRenderer->SetSprite("fullbar.png");
	FrontRenderer->SetComponentLocation({ Pos.X - 165, Pos.Y  });
	FrontRenderer->SetPivotValue({ 0.f, 0.5f });
	FrontRenderer->SetOrder(ERenderOrder::UI_Back);
	FrontRenderer->SetActive(false);
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
	FrontScale = { 300, 95 };
	FrontRenderer->SetComponentScale({ Scale.X * HpRatio, Scale.Y });
}

void ABossHpBar::SetDisplay(bool _Value)
{
	BackRenderer->SetActive(_Value);
	FrontRenderer->SetActive(_Value);
}

ABossHpBar::~ABossHpBar()
{
}