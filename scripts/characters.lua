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
        spritesheet = nil
    else 
        spritesheet = Spritesheet:new(arg_table.spritesheet)
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
        arts = arg_table.arts
    }, CharacterMeta)
end


Bob = Character:new{
    name = "Bob",
    hp = 69,
    aim = 69,
    movement = 3,
    hp_growth = 0.3,
    aim_growth = 0.7
}

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