Tiles = {}

Tile = {
    name = "DEFAULT_TILE",
    tileset = "DEFAULT_TILESET",
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
        index = arg_table.index,
        tileset = arg_table.tileset,
        movement = arg_table.movement,
        dodge = arg_table.dodge,
        heal = arg_table.dodge,
        walkable = arg_table.walkable,
        flyable = arg_table.flyable
    }, TileMeta))
end

blank_tile = Tile:new({
    name = "blank_tile",
    index = 0,
    movement = 0
})

v_tile = Tile:new({
    name = "v_tile",
    index = 2,
    movement = 0
})

x_tile = Tile:new({
    name = "x_tile",
    index = 1,
    movement = 0
})