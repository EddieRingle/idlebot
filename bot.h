/* 
 * File:   bot.h
 * Author: eddie
 *
 * Created on August 20, 2009, 10:08 AM
 */

#ifndef _BOT_H
#define	_BOT_H

#include "universal_include.h"
#include "irc.h"

class Bot {
public:
	Bot();
	Bot(const Bot& orig);
	virtual ~Bot();

	int run();

	IRC *irc;
private:

};

#endif	/* _BOT_H */

