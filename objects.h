#pragma once

#include <map>
#include <functional>
#include <variant>
#include <optional>
#include <memory>
#include <stdexcept>
#include <windows.h>

#ifndef OBJECTS_H
#define OBJECTS_H

#ifndef INVENTORY_ON
#define INVENTORY_ON true;
#endif INVENTORY_ON

namespace frame {
	class item {
		public:
			int weight;
			std::string name;

			item(std::string nme) : name(nme) {};
	};

	class relic : public item {
		//first int is bonus type, second is amount of bonus
		public:
			std::map<int, int> bonuses;
	};

	class weapon : public item {
		//first int is bonus type, second is amount of bonus
		public:
			int attack;

	};

	class controllableObj {
	public:
		std::map<int, std::function<void()>> inputMappings;

		controllableObj(std::map<int, std::function<void()>> mappings) : inputMappings(mappings) {};

		~controllableObj() {};

		bool processObjInput(int keycode) {
			this->inputMappings[keycode]();
		};
	};

	class player2D : public controllableObj {
		public:
			vector2 position;
			int direction;
			std::optional<std::vector<std::variant<relic, weapon>>> inventory;

			player2D(std::map<int, std::function<void()>> mapping, vector2 pos, int dir, std::optional<std::vector<std::variant<relic, weapon>>> Inventory) : controllableObj(mapping), position(pos), direction(dir), inventory(Inventory) {};

	};
}

#endif OBJECTS_H