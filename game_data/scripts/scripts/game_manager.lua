local game_manager = {}

function game_manager:start_game()

  local exists = sol.game.exists("save1.dat")
  local game = sol.game.load("save1.dat")
  if not exists then
    -- Initialize a new savegame.
    game:set_max_life(12)
    game:set_life(game:get_max_life())
    game:set_ability("lift", 2)
    game:set_ability("sword", 1)
  end
  game:start()
end

return game_manager

