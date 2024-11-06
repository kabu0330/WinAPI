#include "PreCompile.h"
#include "HeartUI.h"
#include <EngineCore/SpriteRenderer.h>
#include "Player.h"

AHeartUI::AHeartUI()
{
	for (int i = 0; i < APlayer::GetHeartMax() / 2; i++)
	{
		USpriteRenderer* Sprite = CreateDefaultSubObject<USpriteRenderer>();
		Sprite->SetCameraEffect(false); // 카메라 이동에 영향을 받지 않음. 윈도우 위치에 고정
		Renders.push_back(Sprite);
	}
}

void AHeartUI::SetTextSpriteName(const std::string _Text)
{
	TextSpriteName = _Text;
	for (int i = 0; i < Renders.size(); i++)
	{
		Renders[i]->SetSprite(TextSpriteName);
	}
}

void AHeartUI::SetOrder(int _Order)
{
	for (int i = 0; i < Renders.size(); i++)
	{
		Renders[i]->SetOrder(_Order);
	}
}

void AHeartUI::SetPlayerHp(int _PlayerHp)
{
	int HeartMax = APlayer::GetHeartMax() / 2;
	if (Renders.size() <= _PlayerHp)
	{
		MSGASSERT("함수 호출 이전에 PlayerHp가 최대체력 이상으로 설정된 값이 있습니다.");
		return;
	}

	// 최대 Hp를 먼저 보여주고
	FVector2D Pos = FVector2D::ZERO;
	for (int i = 0; i < HeartMax; i++)
	{
		Renders[i]->SetSprite(TextSpriteName, 2); // Full Heart
		Renders[i]->SetComponentScale(UIScale);
		Renders[i]->SetComponentLocation(Pos);
		Pos.X += UIScale.X;
		Renders[i]->SetActive(true);
	}

	// 여기서부터 감소하는 HP를 보여준다.
	Pos = FVector2D::ZERO;
	if (0 == _PlayerHp) // 사망시 보여질 UI
	{
		Renders[0]->SetSprite(TextSpriteName, 2); // Blank Heart
		Renders[0]->SetComponentScale(UIScale);
		Renders[0]->SetComponentLocation(Pos);
		Renders[0]->SetActive(true);
		return;
	}

	float HeartCount = static_cast<float>(_PlayerHp) / 2; // 7 / 2 = 3.5
	int FullHeart = static_cast<int>(HeartCount); // 3
	float HalfHeart = HeartCount - static_cast<float>(FullHeart); // 0.5

	for (int i = 0; i < FullHeart; i++)
	{
		Renders[i]->SetSprite(TextSpriteName, 0); // Full Heart
		Renders[i]->SetComponentScale(UIScale);
		Renders[i]->SetComponentLocation(Pos);
		Pos.X += UIScale.X;
		Renders[i]->SetActive(true);
	}

	if (0 == HalfHeart) // Hp가 짝수면 리턴
	{
		return;
	}
	for (int i = FullHeart; i < FullHeart + 1; i++)
	{
		Renders[i]->SetSprite(TextSpriteName, 1); // Half Heart
		Renders[i]->SetComponentScale(UIScale);
		Renders[i]->SetComponentLocation(Pos);
		Pos.X += UIScale.X;
		Renders[i]->SetActive(true);
	}

}

void AHeartUI::BeginPlay()
{
	Super::BeginPlay();
}

void AHeartUI::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

AHeartUI::~AHeartUI()
{
}
