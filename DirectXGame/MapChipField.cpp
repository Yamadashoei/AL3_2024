#include "MapChipField.h"
#include <cassert> //assert呼び出し
#include <fstream> //CSVファイル読み込み
#include <map>
#include <sstream> //CSVの文字列ストリームにコピー

namespace {
std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock}
};
}

void MapChipField::ReseMapChipData() {
	// マップチップデータをリセット 02_04 p14
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {
	// マップチップデータをリセット 02_04 p15
	ReseMapChipData();

	// ファイルを開く 02_04 p15
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// マップチップCSV 02_04 p15
	std::stringstream mapChipCsv;
	// ファイルの内容を文字列ストリームにコピー 02_04 p15
	mapChipCsv << file.rdbuf();
	// ファイルを閉じる 02_04 p15
	file.close();

	// CSVからマップチップデータを読み込む 02_04 p16
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		std::string line;
		getline(mapChipCsv, line);

		//  02_04 p16
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) {
	return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0); }
