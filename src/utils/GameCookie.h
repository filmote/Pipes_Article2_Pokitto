#pragma once

#include "Pokitto.h"
#include "PokittoCookie.h"

class GameCookie : public Pokitto::Cookie {
    
    public:
    
    	byte initialised;
        byte level5x5 = 0;
        byte level6x6 = 0;
        byte level7x7 = 0;
        byte level8x8 = 0;
        byte level9x9 = 0;


	public:

		void initialise() {

			initialised = COOKIE_INITIALISED;

			level5x5 = 0;
			level6x6 = 0;
			level7x7 = 0;
			level8x8 = 0;
			level9x9 = 0;
			saveCookie();

		}

		void updateLevel(byte level, byte index) {

			switch (level) {

				case PUZZLE_5X5:
					level5x5 = index;
					break;

				case PUZZLE_6X6:
					level6x6 = index;
					break;

				case PUZZLE_7X7:
					level7x7 = index;
					break;

				case PUZZLE_8X8:
					level8x8 = index;
					break;

				case PUZZLE_9X9:
					level9x9 = index;
					break;
					
			}

			saveCookie();

		}

		byte getLevelIndex(byte level) {

			switch (level) {

				case PUZZLE_5X5:
					return level5x5;

				case PUZZLE_6X6:
					return level6x6;

				case PUZZLE_7X7:
					return level7x7 ;

				case PUZZLE_8X8:
					return level8x8;

				case PUZZLE_9X9:
					return level9x9;
					
			}			
		}

};