#pragma once
#include <vector>
#include <string>
#include <functional>
#include "Position.h"

struct Actor;
struct Player;
struct BlockSource;
struct Scoreboard;
struct BlockPalette;
struct ItemActor;
struct ItemStack;
struct SPSCQueue;
struct Spawner {
	ItemActor* spawnItem(BlockSource* bs, ItemStack* item, Vec3* pos);
};

struct Level {
	//��ȡ����Դ û���ά�ȷ��ؿ�ָ��
	BlockSource* getBlockSource(int did);
	void forEachPlayer(const std::function<bool(Player*)>&);
	Scoreboard* getScoreBoard();
	unsigned getSeed();
	std::string getPlayerNames();
	Actor* fetchEntity(uintptr_t id);
	Player* getPlayerByXuid(const std::string& xuid);
	BlockPalette* getBlockPalette();
	Spawner* getSpawner();
};
