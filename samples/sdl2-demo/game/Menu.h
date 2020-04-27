//
//  Menu.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 5/15/15.
//
//

#pragma once

#include <utility>
#include <vector>

#include "engine/Scene.h"
#include "engine/BakedFont.h"

class Menu : public Scene {
public:
	class BaseItem {
	protected:
		std::string m_label;
	public:
		explicit BaseItem(std::string label) : m_label( std::move( label )) {}

		virtual ~BaseItem() = default;

		virtual std::string label() { return m_label; }

		virtual void enter(Scene& scene) {}

		virtual void exit(Scene& scene) {}

		virtual void update(Scene& scene, float delta) {}

		virtual bool action_select(Scene& scene) { return false; }

		virtual bool action_left(Scene& scene) { return false; }

		virtual bool action_right(Scene& scene) { return false; }
	};

protected:
	BakedFontRef m_font;

	uint16_t m_current_item;

	std::vector<std::unique_ptr<BaseItem>> m_menu_items;
	std::string m_previous_scene;
protected:
	void onShow(const std::string& previous_name, const SceneRef previous) override;

	template<class C, typename ...Args>
	void add(Args&& ...args)
	{
		m_menu_items.emplace_back( new C( std::forward<Args>( args )... ));
	}

	void handleBack();

public:
	explicit Menu(Renderer& rend);

	void update(const InputManager& input, float delta) override;

	void render() override;
};