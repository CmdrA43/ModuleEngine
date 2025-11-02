#include "frame.h"

frame_app_entry_point{

	std::map<int, std::function<void()>> map;
	//Player Definition
	frame::player2D player(map, frame::vector2(0, 0), 0, std::nullopt);

	 map = {
		{22, [&]() {player.position.y += 1; }},
		{0, [&]() {player.position.x -= 1; }},
		{18, [&]() {player.position.y -= 1; }},
		{3, [&]() {player.position.x += 1; }}
	 };

	 player.inputMappings = map;
	 //End Player Definition

	frame::Game::setGameUpdate([&](float delta) {
		wchar_t charBuffer[256];
		swprintf(charBuffer, 256, L"delta: %f\n", frame::Game::getInstance().deltaTime.count());
		OutputDebugString(charBuffer);

		

		if((player.position.x >= -32) && (player.position.x < 32)){
			if ((player.position.y > -18) && (player.position.y < 18)) {
				frame::Renderer::FillRectangle({(int)((player.position.x + 64) * 10), (int)((player.position.y + 36) * 10), 20, 20}, {200, 0, 0});
			}
		}

	}
	);

	frame::Renderer::SetClearColor({200, 100, 0});

	frame::Game::start();

	return 0;
}