#pragma once
#include "Object.h"
#include "EnginePath.h"

// Ό³Έν : 
class UEngineResources : public UObject
{
public:
	// constrcuter destructer
	UEngineResources();
	~UEngineResources();

	// delete Function
	UEngineResources(const UEngineResources& _Other) = delete;
	UEngineResources(UEngineResources&& _Other) noexcept = delete;
	UEngineResources& operator=(const UEngineResources& _Other) = delete;
	UEngineResources& operator=(UEngineResources&& _Other) noexcept = delete;

protected:

private:
	UEnginePath Path;
};

