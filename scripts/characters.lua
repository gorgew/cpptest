characters = {}

Character = {
    name = "DEFAULT_NAME",
    spritesheet = "DEFAULT_SHEET.png",
    stats = Stats,
    equipment = {},
    skills = {},
    arts = {},
    flying = false
}

CharacterMeta = { __index = Character }

function Character:new(arg_table)
    table.insert(characters, setmetatable({
        name = arg_table.name,
        spritesheet = arg_table.spritesheet,
        stats = Stats:new(arg_table)
        ,
        {
            hp_growth = arg_table.hp_growth,
            aim_growth = arg_table.aim_growth,
        },
        equipment = arg_table.equipment,
        skills = arg_table.skills,
        arts = arg_table.arts
    }, CharacterMeta))
end


Bernie = Character:new{
    name = "Bernie",
    hp = 69,
    aim = 69,
    movement = 1,
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