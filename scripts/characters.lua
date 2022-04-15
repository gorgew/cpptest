Characters = {}

Roster = {"Bob"}

Character = {
    spritesheet = "resources/DEFAULT_CHARACTER.png",
    sheet_size = 2,
    sheet_x = 32,
    sheet_y = 32,
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
    Characters[arg_table.name] = setmetatable({
        spritesheet = arg_table.spritesheet,
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
    movement = 2,
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