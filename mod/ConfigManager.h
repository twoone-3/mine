#pragma once
#pragma warning(disable:4996)
#include <third-party/Nlohmann/json.hpp>

using Json = nlohmann::json;

class ConfigManager {
public:
	ConfigManager(const char* fname, const Json& default_value = nullptr);
	~ConfigManager();
	Json& getJson();
	Json& operator[](const std::string& key);
private:
	Json json_;
	FILE* file_;
};

ConfigManager::ConfigManager(const char* fname, const Json& default_value) {
	json_ = default_value;
	FILE* f = fopen(fname, "rb");
	if (f) {
		fseek(f, 0, SEEK_END);
		long size = ftell(f);
		char* text = new char[size + 1];
		rewind(f);
		fread(text, sizeof(char), size, f);
		text[size] = '\0';
		json_ = Json::parse(text);
		delete[] text;
	}
	else {
		f = fopen(fname, "wb");
		fputs(json_.dump(4).c_str(), f);
	}
	file_ = f;
}

ConfigManager::~ConfigManager() {
	if (file_)
		fclose(file_);
}

Json& ConfigManager::getJson() {
	return json_;
}

Json& ConfigManager::operator[](const std::string& key) {
	return json_.operator[](key);
}
