Tiles = {}

Tile = {
    name = "DEFAULT_TILE",
    tilemap = "default_map.png",
    index = 0,
    movement = 0,
    dodge = 0,
    heal = 0,
    walkable = true,
    flyable = true
}

TileMeta = { __index = Tile}

function Tile:OnTurnStart()

end

function Tile:OnTurnEnd()

end

function Tile:OnEnter()

end

function Tile:OnExit()

end

function Tile:new(arg_table)
    table.insert(Tiles, setmetatable({
        name = arg_table.name,
        tilemap = arg_table.tilemap,
        movement = arg_table.movement,
        walkable = arg_table.walkable,
        flyable = arg_table.flyable
    }, TileMeta))
end

debug_wall = Tile:new({
    name = "debug_wall",
    index = 1,
    movement = -99
})