#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <fstream>
#include "mc/json_tool.h"
#include "mc/tool.h"
#include "mc/Block.h"

using namespace std;

static vector<pair<Block**, size_t>> g_blocks;
static size_t g_weight_sum = 0;
constexpr auto CONFIG_PATH = "./plugins/mine.json";
// 液体固化
HOOK(solidify, void, "?solidify@LiquidBlock@@IEBAXAEAVBlockSource@@AEBVBlockPos@@1@Z",
	uintptr_t _this, BlockSource* bs, BlockPos* bp1, BlockPos* bp2) {
	original(_this, bs, bp1, bp2);
	short bid = bs->getBlock(bp1)->getBlockLegacy()->getBlockItemID();
	if (bid == 4 || bid == 1) {//Stone and Cobblestone
		size_t random_number = static_cast<size_t>(rand() % g_weight_sum);
		for (auto& [block, weight] : g_blocks) {
			if (random_number < weight) {
				bs->setBlock(bp1, *block);
				break;
			}
			random_number -= weight;
		}
	}
}
BOOL WINAPI DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
	if (reason == 1) {
		srand(static_cast<unsigned>(time(0)));
		Json config = {
			{"Cobblestone", 200},
			{"CoalOre", 20},
			{"CopperOre", 15},
			{"IronOre", 10},
			{"GoldOre", 5},
			{"EmeraldOre", 3},
			{"RedStoneOre", 10},
			{"LapisOre", 10},
			{"DiamondOre", 2},
		};
		ifstream file(CONFIG_PATH);
		if (!file) {
			cerr << "[mine] mine.json was not found, it has been created automatically" << endl;
			ofstream newfile(CONFIG_PATH);
			newfile << config.dump(4);
			newfile.close();
		}
		else {
			try {
				config = Json::parse(file);
			}
			catch (const std::exception& e) {
				cerr << e.what() << endl;
			}
		}
		for (auto& [key, val] : config.items()) {
			//未初始化，取值为nullptr，需要运行时再取
			Block** block = (Block**)SYM(("?m" + key + "@VanillaBlocks@@3PEBVBlock@@EB").c_str());
			if (!block) {
				cerr << "[mine] invalid block: " << key << endl;
				continue;
			}
			g_blocks.push_back({ block, val.get<size_t>() });
			g_weight_sum += val.get<size_t>();
		}
		for (auto& [key, val] : config.items()) {
			cout << "[mine] " << key << " : "
				<< static_cast<double>(val.get<size_t>() * 100) / g_weight_sum
				<< '%' << endl;
		}
		file.close();
	}
	return TRUE;
}