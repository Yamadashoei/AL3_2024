#pragma once
#include "Model.h"          //モデルヘッダ
#include "WorldTransform.h" //ワールドトランスフォームヘッダ

enum class MapChipType { // 02_04 p9
	kBlank,              // 空白
	kBlock               // ブロック
};

struct MapChipData { // 02_04 p10
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {
public:
	void ReseMapChipData(); // 02_04 p14
	// 02_04 p15
	void LoadMapChipCsv(const std::string& filePath);

	uint32_t GetNumBlockVirtical() const { return kNumBlockVirtical; }
	uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; }

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

private:
	// 1ブロックサイズ 02_04 p8
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	// ブロックの個数 02_04 p8
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipField* mapChipField_;
	MapChipData mapChipData_;
};
