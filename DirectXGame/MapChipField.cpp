#include "MapChipField.h"

namespace {

std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};

}

void MapChipField::ResetMapChipData() {
	_mapChipData.data.clear();
	_mapChipData.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : _mapChipData.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {
	// reset
	ResetMapChipData();

	// load csv file
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// mapChip Csv
	std::stringstream mapChipCsv;

	// file copy
	mapChipCsv << file.rdbuf();

	// close file
	file.close();

	// load MapChipDate from CSV
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		std::string line;
		getline(mapChipCsv, line);

		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				_mapChipData.data[i][j] = mapChipTable[word];
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
	return _mapChipData.data[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0); }

uint32_t MapChipField::GetNumBlockVirtical() {return kNumBlockVirtical;}

uint32_t MapChipField::GetNumBlockHorizontal() { return kNumBlockHorizontal; }
