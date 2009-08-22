/* 
 * File:   bot.cpp
 * Author: eddie
 * 
 * Created on August 20, 2009, 10:08 AM
 */

#include "bot.h"

Bot::Bot()
{
	this->irc = new IRC();
}

Bot::Bot(const Bot& orig)
{
	this->irc = orig.irc;
}

Bot::~Bot()
{
	delete this->irc;
	this->irc = NULL;
}

int Bot::run()
{
	if (this->irc->connect("irc.2600.net","[bot]Orion") != 0)
		return -1;
	this->irc->join("#onlink");
	return 0;
}