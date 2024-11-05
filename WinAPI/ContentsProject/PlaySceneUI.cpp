#include "PreCompile.h"
#include "PlaySceneUI.h"
#include "EngineBase/EngineDebug.h"

APlaySceneUI::APlaySceneUI()
{
	for (int i = 0; i < 10; i++)
	{
		USpriteRenderer* Sprite = CreateDefaultSubObject<USpriteRenderer>();
		Sprite->SetCameraEffect(false); // ī�޶� �̵��� ������ ���� ����. ������ ��ġ�� ����
		Renders.push_back(Sprite);
	}
	int a = 0;
}

void APlaySceneUI::SetTextSpriteName(const std::string _Text)
{
	TextSpriteName = _Text;
	for (int i = 0; i < Renders.size(); i++)
	{
		Renders[i]->SetSprite(TextSpriteName);
	}
}

void APlaySceneUI::SetOrder(int _Order)
{
	for (int i = 0; i < Renders.size(); i++)
	{
		Renders[i]->SetOrder(_Order);
	}
}

void APlaySceneUI::SetValue(int _Value)
{
	std::string Number = std::to_string(_Value);
	if (Renders.size() <= Number.size())
	{
		MSGASSERT("UI�� RenderSize�� �ʰ��߽��ϴ�.");
		return;
	}

	FVector2D Pos = FVector2D::ZERO;
	for (int i = 0; i < Number.size(); i++)
	{
		char Value = Number[i] - '0';
		Renders[i]->SetSprite(TextSpriteName, Value);
		Renders[i]->SetComponentScale(UIScale);
		Renders[i]->SetComponentLocation(Pos);
		Pos.X += UIScale.X;
		Renders[i]->SetActive(true);
	}

	for (int i = Number.size(); i < Renders.size() ; i++)
	{
		Renders[i]->SetActive(false);
	}
}

void APlaySceneUI::BeginPlay()
{
	Super::BeginPlay();
}

void APlaySceneUI::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

APlaySceneUI::~APlaySceneUI()
{
}