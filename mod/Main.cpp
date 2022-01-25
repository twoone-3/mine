#include <LoggerAPI.h>
#include <HookAPI.h>
#include <third-party/Nlohmann/fifo_json.hpp>
#include <Global.h>
#include <MC/Block.hpp>
#include <MC/BlockLegacy.hpp>
#include <MC/BlockSource.hpp>

using namespace std;

static vector<pair<Block**, size_t>> g_blocks;
static size_t g_weight_sum = 0;
constexpr const char* CONFIG_PATH = "./plugins/mine.json";
Logger logger("mine");

void entry() {
	srand(static_cast<unsigned>(time(0)));
	fifo_json config = {
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
		logger.error("mine.json wasn't found, it will be created automatically");
		ofstream newfile(CONFIG_PATH);
		newfile << config.dump(4);
		newfile.close();
	}
	else {
		try {
			config = fifo_json::parse(file);
		}
		catch (const std::exception& e) {
			cerr << e.what() << endl;
		}
	}
	for (auto& [key, val] : config.items()) {
		//未初始化，取值为nullptr，需要运行时再取
		string sym("?m" + key + "@VanillaBlocks@@3PEBVBlock@@EB");
		Block** block = reinterpret_cast<Block**>(SYM(sym.c_str()));
		if (!block) {
			logger.error("[mine] invalid block: {}", key);
			continue;
		}
		g_blocks.push_back({ block, val.get<size_t>() });
		g_weight_sum += val.get<size_t>();
	}
	for (auto& [key, val] : config.items()) {
		double possibility = static_cast<double>(val.get<size_t>() * 100) / g_weight_sum;
		logger.info("{} : {:.2f}%", key, possibility);
	}
	file.close();
	//Event::LiquidSpreadEvent::subscribe(
	//	[](Event::LiquidSpreadEvent e) {
	//		bool result = e.call();
	//		short bid = e.mBlockInstance.getBlock()->getLegacyBlock().getBlockItemId();
	//		//Stone and Cobblestone
	//		if (bid == 4 || bid == 1) {
	//			size_t random_number = static_cast<size_t>(rand() % g_weight_sum);
	//			for (auto& [block, weight] : g_blocks) {
	//				if (random_number < weight) {
	//					e.mBlockInstance.getBlockSource()->setBlockSimple(e.mBlockInstance.getPosition(), **block);
	//					break;
	//				}
	//				random_number -= weight;
	//			}
	//		}
	//		return result;
	//	}
	//);
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
	if (bid == 4 && bid == 1) {//Stone and Cobblestone
		size_t random_number = rand() % g_weight_sum;
		for (auto [block, weight] : g_blocks) {
			if (random_number < weight) {
				bs.setBlockSimple(bp1, **block);
				break;
			}
			random_number -= weight;
		}
	}
}

//BOOL WINAPI DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
//	if (reason == 1) {
//	}
//	return TRUE;
//}