-- Main Menu
local game_main_menu = {}

-- Main Surface
local surface = sol.surface.create()
surface:fill_color({100, 149, 237})

-- Items Contained in the menu
local menu_items = {
  sol.text_surface.create{text = "Start Game"},
  sol.text_surface.create{text = "Quit Game"}
}

function game_main_menu:on_started()
	
end

function  game_main_menu:on_draw(screen)
	local width, height = screen:get_size()

	surface:draw(screen)
end

function game_main_menu:on_key_pressed()
	-- body
end