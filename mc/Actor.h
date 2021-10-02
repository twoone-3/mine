#pragma once
#include <vector>
#include "Position.h"
#include "span.h"

struct BlockSource;
struct ItemStack;
struct Level;
struct Tag;
struct NetworkIdentifier;
struct Container;
struct ScorePacketInfo;
struct Abilities;
enum class PlayerPermissionLevel : uint8_t {
	Visitor,
	Member,
	Operator,
	Custom
};
enum class CommandPermissionLevel : uint8_t {
	Any,
	GameMasters,
	Admin,
	Host,
	Owner,
	Internal 
};
struct Actor {
	//��ȡ����������Ϣ
	std::string getNameTag();
	//��������������Ϣ
	void setNameTag(const std::string&);
	//�������������Ƿ�ɼ�
	void setNameTagVisible(bool visible);
	//��ȡ���ﵱǰ����ά��ID
	int getDimensionId();
	//��ȡ���ﵱǰ��������
	Vec3* getPos();
	//��ȡ����֮ǰ��������
	Vec3* getPosOld();
	//�Ƿ����Ƴ�
	bool isRemoved();
	//�Ƿ�����
	bool isStand();
	//ȡ����Դ
	BlockSource* getRegion();
	ItemStack* getArmor(int slot);
	//��ȡʵ������
	unsigned getEntityTypeId();
	//��ȡ��ѯ��ID
	uintptr_t getUniqueID();
	//��ȡʵ������
	std::string getEntityTypeName();
	//��������
	uintptr_t updateAttrs();
	//��ȡ��ͼ��Ϣ
	Level* getLevel();
	//���һ��״̬
	uintptr_t addEffect(uintptr_t ef);
	//��ȡ����ֵ
	int getHealth();
	int getMaxHealth();
	void setHealth(int value);
	void setMaxHealth(int value);
	//��ȡ����
	ItemStack* getOffHand();
	Tag* save();
	//���ô�С
	void setSize(float f1, float f2);
	//�Ƿ�Ǳ��
	bool isSneaking();
	//��ȡ״̬�б�
	auto getAllEffects();
	//����
	void teleport(Vec3* target, int did);
	//������ǩ
	bool addTag(const std::string& str);
	//�Ƴ���ǩ
	bool removeTag(const std::string& str);
	//��ȡ��ǩ
	span<std::string> getTags();
	//ɱ��ʵ��
	void kill();
};
struct Mob : Actor {};
struct Player : Mob {
	//��ȡ���uuid
	std::string getUuid();
	//��ȡ���xuid
	std::string& getXuid();
	//��ȡ�����ʶ��
	NetworkIdentifier* getClientId();
	//��ȡ����
	Container* getInventory();
	//��ȡװ������
	Container* getArmorContainer();
	//��ȡĩӰ��
	Container* getEnderChestContainer();
	//����һ��װ��
	uintptr_t setArmor(int i, ItemStack* item);
	//���ø���
	uintptr_t setOffhandSlot(ItemStack* item);
	//���һ����Ʒ
	void addItem(ItemStack* item);
	//���ӵȼ�
	void addLevel(int level);
	//��ȡ��ǰѡ�еĿ�λ��
	int getSelectedItemSlot();
	//��ȡ��ǰ��Ʒ
	ItemStack* getSelectedItem();
	//��ȡ������Ʒ
	ItemStack* getInventoryItem(int slot);
	//��ȡ����Ȩ��
	PlayerPermissionLevel getPlayerPermissionLevel();
	//������Ϸʱ����Ȩ��
	void setPermissions(PlayerPermissionLevel m);
	//��ȡ�������
	Abilities* getAbilities();
	//��ȡ�豸id
	std::string getPlatformOnlineId();
	//��ȡ�豸ϵͳ����
	unsigned getPlatform();
	//���ͱ���
	void sendInventroy();
	//ˢ������
	void resendAllChunks();
	//�������ݰ�
	void sendPacket(uintptr_t pkt);
	unsigned sendModalFormRequestPacket(const std::string& str);
	void sendTransferPacket(const std::string& address, unsigned short port);
	void sendDisconnectPacket(const std::string& msg);
	void sendTextPacket(int mode, const std::string& msg);
	void sendCommandRequestPacket(const std::string& cmd);
	void sendBossEventCodePacket(std::string name, float per, int eventtype);
	void sendsetDisplayObjectivePacket(const std::string& title, const std::string& name = "name");
	void sendSetScorePacket(char type, const std::vector<ScorePacketInfo>& slot);
};
//�Ƿ�Ϊ���
bool IsPlayer(Actor* ptr);
