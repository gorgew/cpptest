Spritesheet = {
    name = "DEFAULT_TILESET",
    path = "resources/default_tileset.png",
    count = 1,
    width = 16,
    height = 16,
    animations = {}
}

SpritesheetMeta = {__index = Spritesheet}

function Spritesheet:add_animation(name, start, stop, timings, loop)
    loop = loop or false
    self.animations[name] = {start, stop, timings}
end

function Spritesheet:new(arg_table)
    return setmetatable({
        name = arg_table.name, 
        path = arg_table.path,
        count = arg_table.count,
        width = arg_table.width,
        height = arg_table.height
    }, SpritesheetMeta)
end