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

Stats = {
    curr_hp = 0,
    max_hp = 0,
    aim = 0,
    movement = 0,
    armor = 0
}
Stats.meta = {}

function Stats:add(s1, s2)
    --print("", dump(s1))
    print("", dump(self)) 

    return setmetatable({
        
        curr_hp = s1.curr_hp + s2.curr_hp,
        
        max_hp = s1.max_hp + s2.max_hp,
        aim = s1.aim + s2.aim,
        movement = s1.movement + s2.movement,
        armor = s1.armor + s2.armor
        
    }, Stats.meta)
end

Stats.meta.__add = Stats.add

function Stats:new(arg_table)
    local stat = {
        curr_hp = arg_table.hp == nil and 0 or arg_table.hp,
        max_hp = arg_table.hp == nil and 0 or arg_table.hp,
        aim = arg_table.aim == nil and 0 or arg_table.aim,
        movement = arg_table.movement == nil and 0 or arg_table.movement,
        armor = arg_table.armor == nil and 0 or arg_table.armor

        --[[
        curr_hp = 0,
        max_hp = 0,
        aim =  0,
        movement = 0,
        armor = 0]]
    }
    setmetatable(stat, Stats.meta)
    return stat
end

S1 = Stats:new({})
S2 = Stats:new({})
print(dump(S1))
print(dump(S2))

--Stats:add(S1, S2)

function sum(x, y)
    return {x.x + y.x}
end

local X = {x = 5}
X = sum (X, X)
print(dump(X))