Equipments = {}

Equipment = {
    stats = Stats,
    description = "unused text"
}

--Owner: owner of equipment
function Equipment:onEquip(owner) 
end

--Owner: owner of equipment
function Equipment:onUnequip(owner) 
end

EquipmentMeta = {__index = Equipment}

function Equipment:new(args)
    Equipments[args.name] = setmetatable({
        stats = Stats:new(args),
        description = args.description
    }, EquimentMeta)
    return setmetatable({name = args.name}, EquipmentMeta)
end

Weapon = {}

function Weapon:new(args)
    Equipments[args.name] = setmetatable({
        stats = Stats:new(args),
        description = args.description,
        damage = args.damage,
    }, EquimentMeta)
    return setmetatable({name = args.name}, EquipmentMeta)
end

LeatherBoots = Equipment:new{
    name = "Leather Boots",
    description = "Worn leather boots.",
    movement = 1,
}

Weapon:new{
    name = "Iron Sword",
    description = "An iron sword. Make sure it doesn't rust!",
    damage = 1,
}