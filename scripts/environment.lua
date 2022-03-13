EnvObjs = {}
EnvObj = {
    name = "DEFAULT_ENV_OBJ",
    spritesheet = "resources/DEFAULT_ENV.png",
    sheet_size = 3,
    sheet_x = 32,
    sheet_y = 32,
    world_x = 70,
    world_y = 70,
    flammable = true,
    on_fire = false,
    hp = 3,
    can_interact = false,
    index_at_hp = {1, 2, 3},
    is_billboard = true
}

EnvMeta = { __index = EnvObj }

function EnvObj:interact()
end

function EnvObj:new(args)
    EnvObjs[args.name] = setmetatable({
        spritesheet = args.spritesheet,
        sheet_size = args.sheet_size,
        sheet_x = args.sheet_x,
        sheet_y = args.sheet_y,
        world_x = args.world_x,
        world_y = args.world_y,
        flammable = args.flammable,
        on_fire = args.on_fire,
        hp = args.hp,
        can_interact = args.can_interact,
        index_at_hp = args.index_at_hp,
        is_billboard = args.is_billboard
    }, EnvMeta)
end

TestEnv = EnvObj:new({
    name = "Test",
    can_interact = true
})