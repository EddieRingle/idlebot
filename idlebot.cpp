/* 
 * File:   idlebot.cpp
 * Author: eddie
 *
 * Created on August 20, 2009, 1:12 PM
 */

#include "universal_include.h"
#include "bot.h"

Console *console = new Console();
/*
 * 
 */
int main(int argc, char** argv)
{
	console->WriteLine("Starting IdleBot bot: \"Orion\"...\n\n");
	Bot *orion = new Bot();
	orion->run();
	return(EXIT_SUCCESS);
}

