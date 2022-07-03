#define NOMINMAX
#include "ConfigManager.h"
#include <Global.h>
#include <LoggerAPI.h>
//#include <HookAPI.h>
#include <MC/Block.hpp>
#include <MC/BlockLegacy.hpp>
#include <MC/BlockSource.hpp>

using namespace std;

Logger logger("mine");
vector<pair<Block**, size_t>> g_blocks;
size_t g_weight_sum = 0;
constexpr const char* CONFIG_PATH = "./plugins/mine.json";

void entry() {
	srand(static_cast<unsigned>(time(0)));
	ConfigManager cm(CONFIG_PATH, {
		{"Cobblestone", 100},
		{"Stone", 100},
		{"CoalOre", 20},
		{"CopperOre", 15},
		{"IronOre", 10},
		{"GoldOre", 5},
		{"EmeraldOre", 3},
		{"RedStoneOre", 10},
		{"LapisOre", 10},
		{"DiamondOre", 2},
		});
	for (auto& [key, val] : cm.getJson().items()) {
		//未初始化，取值为nullptr，需要运行时再取
		string sym("?m" + key + "@VanillaBlocks@@3PEBVBlock@@EB");
		Block** block = reinterpret_cast<Block**>(dlsym_real(sym.c_str()));
		if (!block) {
			logger.error("[mine] invalid block: {}", key);
			continue;
		}
		g_blocks.push_back({block, val.get<size_t>()});
		g_weight_sum += val.get<size_t>();
	}
	for (auto& [key, val] : cm.getJson().items()) {
		double possibility = static_cast<double>(val.get<size_t>() * 100) / g_weight_sum;
		logger.info("{} : {:.2f}%", key, possibility);
	}
}

extern "C" _declspec(dllexport) void onPostInit() {
	std::ios::sync_with_stdio(false);
	entry();
}

THook(void, "?solidify@LiquidBlock@@IEBAXAEAVBlockSource@@AEBVBlockPos@@1@Z",
	uintptr_t _this, BlockSource& bs, BlockPos& bp1, BlockPos& bp2) {
	//生成石头
	original(_this, bs, bp1, bp2);
	short bid = bs.getBlock(bp1).getLegacyBlock().getBlockItemId();
	if (bid == 4 || bid == 1) {//Stone and Cobblestone
		size_t random_number = rand() % g_weight_sum;
		for (auto& [block, weight] : g_blocks) {
			if (random_number < weight) {
				bs.setBlockSimple(bp1, **block);
				break;
			}
			random_number -= weight;
		}
	}
}

