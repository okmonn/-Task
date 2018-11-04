#include "Map.h"
#include "../Camera/Camerah.h"
#include "../Object/Chip.h"
#include "../../Func/Func.h"

// マップチップ数
#define CHIP_X 120
#define CHIP_Y 30

// tileのサイズ
#define TILE_X 448
#define TILE_Y 160

// objのサイズ
#define OBJ_X 992
#define OBJ_Y 224

// マップチップサイズ
#define CHIP_SIZE 16

// コンストラクタ
Map::Map(std::weak_ptr<Camera>cam) : 
	cam(cam), threadFlag(true)
{
	map.clear();
	th.clear();

	LoadCsv("Rsc/MapData/Map.tmx");
	Sort();
	Load("Rsc/tileset.png", "tile");
	Load("Rsc/tileset.png", "damage");
	Load("Rsc/objects-Sheet.png", "obj");
}

// デストラクタ
Map::~Map()
{
	threadFlag = false;
	for (auto itr = th.begin(); itr != th.end(); ++itr)
	{
		if (itr->second.joinable() == false)
		{
			itr->second.join();
		}
	}

	for (auto itr = map.begin(); itr != map.end(); ++itr)
	{
		for (auto& i : itr->second)
		{
			func::DeleteImg(i);
		}
	}
}

// CSVの読み込み
int Map::LoadCsv(const std::string & fileName)
{
	FILE* file = nullptr;
	
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		return -1;
	}

	std::string type;
	std::string tmp;
	char n = 0;
	while (std::feof(file) == 0)
	{
		fread(&n, sizeof(n), 1, file);
		if (n == 0x0d || n == 0x0a)
		{
			if (tmp.size() > 0)
			{
				map[type].push_back(std::atoi(tmp.c_str()));
				tmp.clear();
			}
			continue;
		}

		if (n == 0x3c)
		{
			tmp.push_back(n);
			while (n != 0x3e)
			{
				fread(&n, sizeof(n), 1, file);
				tmp.push_back(n);
			}

			if (tmp.find("name") != tmp.npos)
			{
				std::string dummy = tmp.substr(tmp.find("name") + 4 + 2, tmp.size());
				type = dummy.substr(0, dummy.find_first_of('\"'));
			}
			tmp.clear();
		}
		else
		{
			if (n == 0x2c)
			{
				map[type].push_back(std::atoi(tmp.c_str()));
				tmp.clear();
			}
			else
			{
				tmp.push_back(n);
			}
		}
	}

	fclose(file);

	return 0;
}

// ソート
void Map::Sort(void)
{
	for (auto itr = map.begin(); itr != map.end(); ++itr)
	{
		auto tmp = itr->second;
		for (int y = 0; y < CHIP_Y; ++y)
		{
			for (int x = 0; x < CHIP_X; ++x)
			{
				itr->second[x * CHIP_Y + y] = tmp[y * CHIP_X + x];
			}
		}
	}
}

// 画像の読み込み
void Map::Load(const std::string & fileName, const std::string & type)
{
	for (int x = 0; x < CHIP_X; ++x)
	{
		for (int y = 0; y < CHIP_Y; ++y)
		{
			if (map[type][x * CHIP_Y + y] != 0)
			{
				int cnt = 0;
				int num = (type == "obj") ? OBJ_X / CHIP_SIZE : TILE_X / CHIP_SIZE;
				while (true)
				{
					++cnt;
					if (map[type][x * CHIP_Y + y] < num * cnt)
					{
						break;
					}
				}
				Vec2f rect = { (float)(CHIP_SIZE * (num - (num * cnt - map[type][x * CHIP_Y + y]) - 1)), (float)(CHIP_SIZE * (cnt - 1)) };
				Vec2f pos  = { (float)(CHIP_SIZE * x), (float)(CHIP_SIZE * y) };
				Vec2f size = (float)CHIP_SIZE;
				chip[type].push_back(std::make_shared<Chip>(fileName, map[type][x * CHIP_Y + y], cam, rect, pos, size));
			}
		}
	}
}

// 描画
void Map::Draw(const std::string & type)
{
	for (auto itr = chip[type].begin(); itr != chip[type].end(); ++itr)
	{
		(*itr)->Draw();
	}
}
