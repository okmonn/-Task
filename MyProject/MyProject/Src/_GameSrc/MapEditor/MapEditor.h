#pragma once

class MapEditor
{
public:
	// コンストラクタ
	MapEditor();
	// デストラクタ
	~MapEditor();

	// 描画
	void Draw(void);
	
	// 処理
	void UpData(void);

private:
	// マップチップのインデックス
	unsigned int index;
};
