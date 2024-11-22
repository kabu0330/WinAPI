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

	FVector2D Pos = { Global::WindowSize.X * 0.5f, Global::WindowSize.Y * 0.12f };
	Scale = { 300, 60 };
	std::string SpriteName= "ui_bosshealthbar.png";
	BackRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BackRenderer->CreateAnimation("Back", SpriteName, 1, 1);
	BackRenderer->SetComponentLocation(Pos);
	BackRenderer->SetComponentScale(Scale);
	BackRenderer->SetOrder(ERenderOrder::UI_Back);
	BackRenderer->ChangeAnimation("Back");
	BackRenderer->SetActive(false);
	BackRenderer->SetCameraEffect(false);

	FrontRenderer = CreateDefaultSubObject<USpriteRenderer>();
	FrontRenderer->SetSprite("hpbar.png");
	//FrontRenderer->CreateAnimation("Back", SpriteName, 0, 0);
	FrontRenderer->SetComponentLocation({ Pos.X - 105, Pos.Y - 2 });
	FrontRenderer->SetPivotValue({ 0.0f, 0.5f });
	FrontRenderer->SetOrder(ERenderOrder::UI);
	//FrontRenderer->ChangeAnimation("Back");
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
	FrontScale = { 215, 14 };
	FrontRenderer->SetComponentScale({ FrontScale.X * HpRatio, FrontScale.Y });
}

void ABossHpBar::SetDisplay(bool _Value)
{
	BackRenderer->SetActive(_Value);
	FrontRenderer->SetActive(_Value);
}

ABossHpBar::~ABossHpBar()
{
}