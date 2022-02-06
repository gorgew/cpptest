Maps = {}

Map = {
    name = "DEFAULT_MAP",
    width = 5,
    height = 5,
    TerrainMap = {},
    EnvMap = {},
    CharMap = {}
}

MapMeta = { __index = Map }

function Map:new(arg_table)
    Maps[arg_table.name] = setmetatable({
        name = arg_table.name,
        width = arg_table.width,
        height = arg_table.height,
        TerrainMap = arg_table.TerrainMap,
        EnvMap = arg_table.EnvMap,
        CharMap = arg_table.CharMap
    }, MapMeta)
end