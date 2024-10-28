#pragma once

// Ό³Έν :
class ClassName
{
public:
	// constrcuter destructer
	ClassName();
	~ClassName();

	// delete Function
	ClassName(const ClassName& _Other) = delete;
	ClassName(ClassName&& _Other) noexcept = delete;
	ClassName& operator=(const ClassName& _Other) = delete;
	ClassName& operator=(ClassName&& _Other) noexcept = delete;

protected:

private:

};

