Stats = {
    curr_hp = 1,
    max_hp = 1,
    aim = 1,
    movement = 4
}
StatsMeta = { __index = vector }

function Stats:new(arg_table) 
    return setmetatable({
        curr_hp = arg_table.hp,
        max_hp = arg_table.hp,
        aim = arg_table.aim,
        movement = arg_table.movement
    }, StatsMeta)
end