#pragma once
#include <vector>
#include "Tear.h"
#include <EngineCore/Actor.h>

// Ό³Έν :
class ATearManager : public AActor
{
public:
	// constrcuter destructer
	ATearManager();
	~ATearManager();

	// delete Function
	ATearManager(const ATearManager& _Other) = delete;
	ATearManager(ATearManager&& _Other) noexcept = delete;
	ATearManager& operator=(const ATearManager& _Other) = delete;
	ATearManager& operator=(ATearManager&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void Fire(FVector2D _StartPos, FVector2D _Dir);

	std::vector<ATear*>& GetTears() { return Tears; }

protected:

private:
	int poolCount = 30;
	std::vector<ATear*> Tears;
};

