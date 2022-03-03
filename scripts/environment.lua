EnvObjs = {}
EnvObj = {
    name = "DEFAULT_ENV_OBJ",
    flammable = true,
    on_fire = false,
    hp = 5,
    can_interact = false
}

EnvMeta = { __index = EnvObj }

function EnvObj:interact()
end

function EnvObj:new(arg_table)
    EnvObjs[arg_table.name] = setmetatable({
        flammable = arg_table.flammable,
        on_fire = arg_table.on_fire,
        hp = arg_table.hp,
        can_interact = arg_table.can_interact
    }, EnvMeta)
end