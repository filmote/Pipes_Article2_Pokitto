#pragma once

#include "Pokitto.h"
#include "PokittoCookie.h"

class GameCookie : public Pokitto::Cookie {
    
    public:
    
    	uint8_t initialised;
        uint8_t level5x5 = 0;
        uint8_t level6x6 = 0;
        uint8_t level7x7 = 0;
        uint8_t level8x8 = 0;
        uint8_t level9x9 = 0;


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

		void updateLevel(uint8_t level, uint8_t index) {

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

		uint8_t getLevelIndex(uint8_t level) {

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