#pragma once
#include <cstdint>
class MapChipField {
	// blockのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	// blockの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;
};
enum MapChipType {
	kBlank,		//空白
	kBlock,		//ブロック
};