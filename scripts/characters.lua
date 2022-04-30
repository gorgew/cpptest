Characters = {}

Roster = {"Bob"}

Character = {
    spritesheet = Spritesheet:new({
        path = "resources/DEFAULT_CHARACTER.png",
        count = 2,
        width = 32,
        height = 32
    }),
    world_x = 100,
    world_y = 100,
    stats = Stats,
    equipment = {},
    skills = {},
    arts = {},
    flying = false
}

CharacterMeta = { __index = Character }

function Character:new(arg_table)
    local spritesheet_
    if (arg_table.spritesheet == nil) then 
        spritesheet_ = nil
    else 
        spritesheet_ = Spritesheet:new(arg_table.spritesheet)
    end
    Characters[arg_table.name] = setmetatable({
        spritesheet = spritesheet_,
        stats = Stats:new(arg_table)
        ,
        {
            movement = arg_table.movement,
            hp_growth = arg_table.hp_growth,
            aim_growth = arg_table.aim_growth,
        },
        equipment = arg_table.equipment,
        skills = arg_table.skills,
        arts = arg_table.arts,
        world_x = arg_table.world_x,
        world_y = arg_table.world_y
    }, CharacterMeta)
    return Characters[arg_table.name]
end


Bob = Character:new{
    name = "Bob",
    hp = 69,
    aim = 69,
    movement = 3,
    hp_growth = 0.3,
    aim_growth = 0.7,
    spritesheet = {
        path = "resources/rat_wizard.png",
        count = 8,
        width = 64,
        height = 64
    },
    world_x = 200,
    world_y = 200,
}

Bob.spritesheet:add_animation("walk_up", 0, 3, {250, 250, 250, 250})
Bob.spritesheet:add_animation("walk_left", 0, 3, {250, 250, 250, 250})
Bob.spritesheet:add_animation("walk_down", 4, 7, {250, 250, 250, 250})
Bob.spritesheet:add_animation("walk_right", 4, 7, {250, 250, 250, 250})

function dump(o)
    if type(o) == 'table' then
       local s = '{ '
       for k,v in pairs(o) do
          if type(k) ~= 'number' then k = '"'..k..'"' end
          s = s .. '['..k..'] = ' .. dump(v) .. ','
       end
       return s .. '}'
    else
       return tostring(o)
    end
 end

 print("Characters:", dump(characters))