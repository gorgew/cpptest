characters = {}

Character = {}

function Character:new(arg_table) 
    table.insert(characters, {
        name = arg_table.name or "DEFAULT_NAME",
        spritesheet = arg_table.spritesheet or "DEFAULT_SHEET.png",
        stats = Stats:new(arg_table)
        ,
        {
            hp_growth = arg_table.hp_growth or 0,
            aim_growth = arg_table.aim_growth or 0
        },
        equipment = arg_table.equipment or {},
        skills = arg_table.skills or {},
        arts = arg_table.arts

    })
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