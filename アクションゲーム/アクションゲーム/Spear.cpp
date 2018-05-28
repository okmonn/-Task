#include "Spear.h"

const std::string path = "アクション/pike.act";
const int line = 330;

// コンストラクタ
Spear::Spear(Positionf pos, std::weak_ptr<Player>pl, std::weak_ptr<Camera>cam) : pl(pl), cam(cam)
{
	Load(path);
	this->pos = pos;
}

// デストラクタ
Spear::~Spear()
{
}

// 状態のセット
void Spear::SetMode(std::string m, bool r)
{
}

// 描画
void Spear::Draw(void)
{
}

// 処理
void Spear::UpData(void)
{
}
