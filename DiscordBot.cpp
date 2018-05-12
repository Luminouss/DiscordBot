// DiscordBot.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include <string>
#include <list>

#include "Bot.h"



int main()
{
	Bot b;

	while (b.getRunning()) {
		b.Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	return 0;
}
