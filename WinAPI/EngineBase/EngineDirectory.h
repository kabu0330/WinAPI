#pragma once
#include <vector>
#include "EnginePath.h"

// ���� : �̹��� ���ҽ� ����
class UEngineDirectory : public UEnginePath
{
public:
	UEngineDirectory();
	UEngineDirectory(std::string_view _Path);
	UEngineDirectory(std::filesystem::path _Path);
	~UEngineDirectory();

	std::vector<class UEngineFile> GetAllFile(bool _IsRecursive = true);
	std::vector<class UEngineDirectory> GetAllDirectory();

protected:

private:
	
	void GetAllFileRecursive(std::filesystem::path _Path, std::vector<class UEngineFile>& _Result);

};

