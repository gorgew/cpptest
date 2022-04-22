EnvObjs = {}
EnvObj = {
    name = "DEFAULT_ENV_OBJ",
    spritesheet = Spritesheet:new({
        name = "DEFAULT_ENV",
        path = "resources/DEFAULT_ENV.png",
        count = 3,
        width = 32,
        height = 32
    }),
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
    local spritesheet_
    if (args.spritesheet == nil) then 
        spritesheet = nil
    else 
        spritesheet = Spritesheet:new(args.spritesheet)
    end
    EnvObjs[args.name] = setmetatable({
        spritesheet = spritesheet_,
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