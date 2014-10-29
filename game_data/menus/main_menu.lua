-- Main Menu
local game_main_menu = {}

-- Main Surface
local surface = sol.surface.create()
surface:fill_color({100, 149, 237})

local heading = sol.text_surface.create {
	font = "title",
	text = "SOLARUS",
	horizontal_alignment = "center",
	color = {255, 255, 102}
}

local subheading = sol.text_surface.create {
	font = "subtitle",
	text = "A Level Created in",
	horizontal_alignment = "center",
	color = {255, 255, 102}	
}

local subheading2 = sol.text_surface.create {
	font = "subtitle",
	text = "the Procedural Level Designer",
	horizontal_alignment = "center",
	color = {255, 255, 102}	
}

-- Items Contained in the menu
local menu_items = {
  sol.text_surface.create{font = "main_menu", text = "Start Game", horizontal_alignment = "center"},
  sol.text_surface.create{font = "main_menu", text = "Quit Game", horizontal_alignment = "center"}
}

local itemSelected = 1

------------------------------------------------------------------------------------------
-- Local Functions
------------------------------------------------------------------------------------------
local function rebuild_surface()
	local screen_width, screen_height = surface:get_size()
	local text_width, text_height = heading:get_size()

	heading:draw(surface, screen_width/2, 45)
	subheading:draw(surface, screen_width/2, 75)
	subheading2:draw(surface, screen_width/2, 95)

	for i,v in ipairs(menu_items) do
		if itemSelected == i then
			v:set_color({255, 0, 0})
		else
			v:set_color({255, 255, 255})
		end 
		
		text_width, text_height = v:get_size()
		
		v:draw(surface, screen_width/2, (screen_height/2) - (text_height/2) + i*(text_height + 10))
	end
	
	
end


------------------------------------------------------------------------------------------
-- Events
------------------------------------------------------------------------------------------
function game_main_menu:on_started()
	rebuild_surface()
end

function  game_main_menu:on_draw(screen)
	local width, height = screen:get_size()
	
	
	
	surface:draw(screen)
end

function game_main_menu:on_key_pressed()
	if sol.input.is_key_pressed("up") then
		sol.audio.play_sound("cursor")
		if itemSelected == 1 then
			itemSelected = table.getn(menu_items)
		else
			itemSelected = itemSelected - 1
		end
	end

	if sol.input.is_key_pressed("down") then
		sol.audio.play_sound("cursor")
		if itemSelected == table.getn(menu_items) then
			itemSelected = 1
		else
			itemSelected = itemSelected + 1
		end
	end

	if sol.input.is_key_pressed("return") then
		sol.audio.play_sound("ok")
		if menu_items[itemSelected]:get_text() == "Start Game" then
			sol.menu.stop(game_main_menu)
		elseif menu_items[itemSelected]:get_text() == "Quit Game" then
			sol.main.exit()
		end
	end

	rebuild_surface()
end

return game_main_menu
