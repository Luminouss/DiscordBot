#pragma once

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

#include <array>
#include <thread>


enum ChannelType { CHANNEL_PICARTO };

struct Channel {
public:
	ChannelType type;
	std::string name;
	bool online;
	Channel(std::string n, ChannelType t = CHANNEL_PICARTO) :name(n), online(false), type(t) {

	}
};
static std::array<Channel, 13> channels = {
	Channel("Rays"),
	Channel("cynpai"),
	Channel("katsukaze"),
	Channel("winyumi"),
	Channel("BlazinFlizard"),
	Channel("Johnsonist"),
	Channel("kiroh"),
	Channel("LePetiteCafe"),
	Channel("yuukiame"),
	Channel("Lemyzest"),
	Channel("Redblaze27"),
	Channel("AsuraEterna"),
	Channel("Chocpocalypse")
};

struct MethodClass
{
private:
	MethodClass();

public:
	MethodClass(std::ostream * stream)
		: mStream(stream)
	{}

	// Helper Class for reading result from remote host
	size_t write(curlpp::Easy *handle, char* ptr, size_t size, size_t nmemb)
	{
		return  size * nmemb;;
	};

	// Public member vars
	std::ostream * mStream;
};

class Bot
{
public:
	Bot();
	~Bot();

	void Update();
	void setRunning(bool);
	bool getRunning() const;

	MethodClass mObject;
private:
	bool _running;

	bool isUserOnline(const std::string&); 
	bool isUserOnlineTempFix(const std::string &);
};

