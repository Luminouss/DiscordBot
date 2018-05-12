#include "Bot.h"

#include "json/json.h"

#include <sstream>
#include <algorithm>
#include <iterator>




Bot::Bot() : _running(true),mObject(&std::cout) {
}

Bot::~Bot() {

}

void Bot::Update()
{
	for (Channel& channel : channels) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		try
		{
			//std::cout << "checking " << channel.name << std::endl;
			if (channel.online != isUserOnlineTempFix(channel.name)) {
				//std::cout << "online status changed" << std::endl;
				//Online Status Changed
				std::cout << channel.name << " online status changed" << std::endl;

				if (channel.online) {
					//was online now offline
				}
				else {
					//was offline now online
					// Discord Api Call
					curlpp::Cleanup cleaner;
					curlpp::Easy request;

					std::cout << channel.name << " is now online, posting discord message" << std::endl;

					std::list<std::string> headers;
					headers.push_back("Authorization: Bot NDQ0NjQ4Mzc4MTk5MDQ4MjE0.DdhDzg.XojA80Nzr_-vgmFMlBLjNBjgpz8");
					headers.push_back("Content-Type: multipart/form-data");

					curlpp::Forms formdata;
					formdata.push_back(new curlpp::FormParts::Content("content", channel.name + " is online!\nWatch live at : https://picarto.tv/" + channel.name));

					request.setOpt(new curlpp::options::Url("https://discordapp.com/api/v6/channels/439065048628068365/messages"));
					request.setOpt(new curlpp::options::HttpHeader(headers));
					request.setOpt(new curlpp::options::HttpPost(formdata));
					request.setOpt(new curlpp::options::WriteFunction(std::bind(&MethodClass::write, &mObject, &request, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

					request.perform();
					//std::system("clear");
				}
				channel.online = !channel.online;
			}
			std::cout << "Channel " + channel.name << " is " << channel.online << std::boolalpha << " checking next Person" << std::endl;
		}
		catch (curlpp::LogicError & e)
		{
			std::cout << e.what() << std::endl;
		}
		catch (curlpp::RuntimeError & e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}

void Bot::setRunning(bool running)
{
	_running = running;
}

bool Bot::getRunning() const
{
	return _running;
}

bool Bot::isUserOnlineTempFix(const std::string & usrname)
{
	{// Picarto Api Call
		curlpp::Cleanup cleaner;
		curlpp::Easy request;

		curlpp::options::Url url("https://picarto.tv/" + usrname);

		request.setOpt(new curlpp::options::Url(url));
		request.setOpt(new curlpp::options::WriteFunction(std::bind(&MethodClass::write, &mObject, &request, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
		request.perform();

		std::stringstream t;
		t << request;

		std::string ay = t.str().substr(t.str().find("\"live\":"), 11);
		return ay.find("true") != std::string::npos;
	}
	//??
	return false;
}

bool Bot::isUserOnline(const std::string & usrname)
{
	{// Picarto Api Call
		curlpp::Cleanup cleaner;
		curlpp::Easy request;

		curlpp::options::Url url("https://api.picarto.tv/v1/channel/name/" + usrname);

		request.setOpt(new curlpp::options::Url(url));
		request.setOpt(new curlpp::options::HttpHeader({ "Authorization: Bearer eyB5A6cglkNyItCHCFNmZzGW" }));
		//request.setOpt(new curlpp::options::WriteFunction(std::bind(&MethodClass::write, &mObject, &request, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
		request.perform();

		std::stringstream t;
		t << request;

		Json::Value val;
		Json::CharReaderBuilder rbuilder;
		rbuilder["collectComments"] = false;
		std::string errs;
		bool ok = Json::parseFromStream(rbuilder, t, &val, &errs);

		if (!ok)
		{
			std::cout << "Error parsing the string :" << std::endl << errs << std::endl;
		}
		return val["online"].asBool();
	}

}
