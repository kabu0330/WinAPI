#include "PreCompile.h"
#include "HeartUI.h"
#include <EngineCore/SpriteRenderer.h>
#include "Player.h"

AHeartUI::AHeartUI()
{
	for (int i = 0; i < APlayer::GetHeartMax() / 2; i++)
	{
		USpriteRenderer* Sprite = CreateDefaultSubObject<USpriteRenderer>();
		Sprite->SetCameraEffect(false); // ī�޶� �̵��� ������ ���� ����. ������ ��ġ�� ����
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
		MSGASSERT("�Լ� ȣ�� ������ PlayerHp�� �ִ�ü�� �̻����� ������ ���� �ֽ��ϴ�.");
		return;
	}

	// �ִ� Hp�� ���� �����ְ�
	FVector2D Pos = FVector2D::ZERO;
	for (int i = 0; i < HeartMax; i++)
	{
		Renders[i]->SetSprite(TextSpriteName, 2); // Full Heart
		Renders[i]->SetComponentScale(UIScale);
		Renders[i]->SetComponentLocation(Pos);
		Pos.X += UIScale.X;
		Renders[i]->SetActive(true);
	}

	// ���⼭���� �����ϴ� HP�� �����ش�.
	Pos = FVector2D::ZERO;
	if (0 == _PlayerHp) // ����� ������ UI
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

	if (0 == HalfHeart) // Hp�� ¦���� ����
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
