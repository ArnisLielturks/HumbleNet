//
//  MainMenu.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 5/15/15.
//
//

#pragma once

#include "Menu.h"

class HostGame : public Menu {
	int16_t m_maxPlayers = 4;
	std::string m_lobbyName = "My game";
	std::string m_playerName = "Player";
public:
	explicit HostGame(Renderer& rend);

protected:
	void onShow(const std::string& previous_name, const SceneRef previous) override;
};