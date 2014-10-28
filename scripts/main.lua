-- This is the main Lua script of your project.
-- You will probably make a title screen and then start a game.
-- See the Lua API! http://www.solarus-games.org/solarus/documentation/

local game_manager = require("scripts/game_manager")

function sol.main:on_started()
  -- This function is called when Solarus starts.
  print("This is a sample quest for Solarus.")

  local mainSurface = sol.surface.create()
  local menuCounter = 0;

  -- Setting a language is useful to display text and dialogs.
  sol.language.set_language("en")

  -- Show the Solarus logo initially.
  local solarus_logo = require("menus/solarus_logo")
  sol.menu.start(self, solarus_logo)

  -- Show the main menu when the Solarus logo menu is finished.
  solarus_logo.on_finished = function()
    local main_menu = require("menus/main_menu")
    main_menu.on_finished = function()  
      game_manager:start_game()
    end
  end

end