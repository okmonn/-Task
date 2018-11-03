#include "Map.h"
#include "../Camera/Camerah.h"
#include "../../Func/Func.h"

// コンストラクタ
Map::Map(std::weak_ptr<Camera>cam) : 
	cam(cam)
{
	map.clear();

	LoadCsv("Rsc/MapData");
}

// デストラクタ
Map::~Map()
{
}

// CSVの読み込み
int Map::LoadCsv(const std::string & dir)
{
	auto list = func::GetDirFile(dir);

	FILE* file = nullptr;
	int cnt = 0;
	for (auto& fileName : list)
	{
		if (fileName.find_last_of('.csv') >= fileName.size())
		{
			continue;
		}

		if (fopen_s(&file, std::string(dir + "/" + fileName).c_str(), "rb") != 0)
		{
			return -1;
		}

		std::string tmp;
		char n = 0;
		while (std::feof(file) == 0)
		{
			fread(&n, sizeof(n), 1, file);
			if (n == 0x2c)
			{
				map[cnt].push_back(std::atoi(tmp.c_str()));
				tmp.clear();
			}
			else
			{
				tmp.push_back(n);
			}
		}

		++cnt;
	}

	return 0;
}
