/* 
 * File:   idlebot.cpp
 * Author: eddie
 *
 * Created on August 20, 2009, 1:12 PM
 */

#include "universal_include.h"
#include "bot.h"
/*
 * 
 */
int main(int argc, char** argv)
{
	Console *console = new Console();
	console->WriteLine("Starting IdleBot bot: \"Orion\"...\n\n");
	Bot *orion = new Bot();
	orion->run();
	delete console;
	return(EXIT_SUCCESS);
}

