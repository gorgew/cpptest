Stats = {
    hp = 0,
    aim = 0,
    movement = 0
}
StatsMeta = { __index = vector }

function Stats:new(arg_table) 
    return setmetatable({
        hp = arg_table.hp or 0,
        aim = arg_table.aim or 0,
        movement = arg_table.movement or 0
    }, StatsMeta)
end