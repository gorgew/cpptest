Tiles = {}

Tile = {
    name = "DEFAULT_TILE",
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
        movement = arg_table.movement,
        walkable = arg_table.walkable,
        flyable = arg_table.flyable
    }, TileMeta))
end

