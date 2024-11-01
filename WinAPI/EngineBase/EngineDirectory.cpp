#include "PreCompile.h"
#include "EngineDirectory.h"
#include "EngineFile.h"

UEngineDirectory::UEngineDirectory()
	: UEnginePath()
{
	// Path = std::filesystem::current_path();
}

UEngineDirectory::UEngineDirectory(std::string_view _Path)
	: UEnginePath(_Path)
{
}

UEngineDirectory::UEngineDirectory(std::filesystem::path _Path)
	: UEnginePath(_Path)
{
}

UEngineDirectory::~UEngineDirectory()
{
}

// Ư�� ���丮�� �ִ� ��� ������ �����´�.
std::vector<class UEngineFile> UEngineDirectory::GetAllFile(bool _IsRecursive /*= true*/)
{															
	std::vector<class UEngineFile> Result;

	// ��θ� �־��ָ� �� ����� ù��° ������ ����Ű�� �ȴ�.
	std::filesystem::directory_iterator Diriter = std::filesystem::directory_iterator(Path);

	// ���ҽ� ��� Result�� �����ϱ� ������ �ݺ��� ��ȸ
	while (false == Diriter._At_end())
	{
		std::filesystem::path FilePath = *Diriter;

		UEnginePath Path = UEnginePath(FilePath);
		if (true == Path.IsDirectory())
		{
			// ���� ������ ���ҽ��� ��� �����ϱ� ���Ѵٸ� true
			if (true == _IsRecursive)
			{
				GetAllFileRecursive(FilePath, Result);
			}

			++Diriter;
			continue;
		}

		// ������ ���� ���ҽ� �����̸� �޸𸮿� �߰�
		Result.push_back(UEngineFile(FilePath));
		++Diriter;
	}

	return Result;
}

std::vector<class UEngineDirectory> UEngineDirectory::GetAllDirectory()
{
	std::vector<class UEngineDirectory> Result;

	// ��θ� �־��ָ� �� ����� ù��° ������ ����Ű�� �ȴ�.
	std::filesystem::directory_iterator Diriter = std::filesystem::directory_iterator(Path);

	while (false == Diriter._At_end())
	{
		std::filesystem::path FilePath = *Diriter;

		UEnginePath Path = UEnginePath(FilePath);
		if (false == Path.IsDirectory())
		{
			++Diriter;
			continue;
		}

		Result.push_back(UEngineDirectory(FilePath));
		++Diriter;
	}

	return Result;
}


// ���� ���丮���� ��� ���ҽ� ������ �����Ѵ�.
void UEngineDirectory::GetAllFileRecursive(std::filesystem::path _Path, std::vector<class UEngineFile>& _Result)
{
	// ��θ� �־��ָ� �� ����� ù��° ������ ����Ű�� �ȴ�.
	std::filesystem::directory_iterator Diriter = std::filesystem::directory_iterator(_Path);

	while (false == Diriter._At_end())
	{
		std::filesystem::path FilePath = *Diriter;

		UEnginePath Path = UEnginePath(FilePath);
		if (true == Path.IsDirectory())
		{
			GetAllFileRecursive(FilePath, _Result);
			++Diriter;
			continue;
		}

		_Result.push_back(UEngineFile(FilePath));
		++Diriter;
	}
}