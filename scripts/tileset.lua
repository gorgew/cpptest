Tilesets = {}

Tileset = {
    name = "DEFAULT_TILESET",
    tileset = "resources/default_tileset.png",
    count = 1,
    width = 16,
    height = 16
}

TilesetMeta = { __index = Tileset}

function Tileset:new(arg_table)
    table.insert(Tilesets, setmetatable({
        name = arg_table.name, 
        tileset = arg_table.tileset,
        count = arg_table.count,
        width = arg_table.width,
        height = arg_table.height
    }, TilesetMeta))
end

DefaultTileset = Tileset:new({count = 12})