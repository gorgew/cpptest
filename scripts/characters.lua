

Characters = {}

Roster = {"Bob"}

Character = {
    spritesheet = Spritesheet:new({
        path = "resources/DEFAULT_CHARACTER.png",
        count = 2,
        width = 32,
        height = 32
    }),
    world_x = 100,
    world_y = 100,
    stats = Stats,
    equipment = {weapon = "", head = "", chest = "", feet = ""},
    arts = {},
    flying = false
}

function Character:add_skill(skill)
    assert(type(skill) == "string", "add_skill requires a string")
    if (PassiveSkills[skill] ~= nil) then 
        table.insert(Characters[self.name]["passive_skills"], skill)
    elseif (ActiveSkills[skill] ~= nil) then
        table.insert(Characters[self.name]["active_skills"], skill)
    else
         assert(false, "add_skill: skill not found: " + skill)
    end
end 

function Character:equip(slot, equipment) 
    --Error checking
    if (Equipments[equipment.name] == nil) then
        assert(false, "equip: equipment not found" )
    end
    if (Characters[self.name]["equipment"][slot] == nil) then
        assert(false, "equip: invalid slot")
    end
    if (Characters[self.name]["equipment"][slot] ~= "") then
        assert(false, "equip: slot already used ")
    end

    if (Equipments[equipment.name] ~= nil and Characters[self.name]["equipment"][slot] == "") then
        Characters[self.name]["equipment"][slot] = equipment.name
        local s1 = Characters[self.name]["stats"]
        local s2 = Equipments[equipment.name]["stats"]

        Characters[self.name]["stats"] = Stats:add(Characters[self.name]["stats"], Equipments[equipment.name]["stats"]) 
    end
end

--Unequip current item in slot and return its name
function Character:unequip(slot) 
    if (Characters[self.name]["equipment"][slot] == nil) then
        assert(false, "equip: invalid slot")
    end
    if (Characters[self.name]["equipment"][slot] == "") then
        assert(false, "equip: slot empty ")
    end

    return {
        name = Characters[self.name]["equipment"][slot] 
    } 
end

CharacterMeta = { __index = Character }

function Character:new(arg_table)
    local spritesheet_
    if (arg_table.spritesheet == nil) then 
        spritesheet_ = nil
    else 
        spritesheet_ = Spritesheet:new(arg_table.spritesheet)
    end
    Characters[arg_table.name] = setmetatable({
        spritesheet = spritesheet_,
        stats = Stats:new(arg_table)
        ,
        {
            hp_growth = arg_table.hp_growth,
            aim_growth = arg_table.aim_growth,
        },
        equipment = arg_table.equipment,
        active_skills = {},
        passive_skills = {},
        arts = arg_table.arts,
        world_x = arg_table.world_x,
        world_y = arg_table.world_y
    }, CharacterMeta)
    return setmetatable({name = arg_table.name}, CharacterMeta)
end

        

Bob = Character:new{
    name = "Bob",
    hp = 69,
    aim = 69,
    movement = 3,
    hp_growth = 0.3,
    aim_growth = 0.7,
    spritesheet = {
        path = "resources/rat_wizard.png",
        count = 8,
        width = 64,
        height = 64
    },
    world_x = 200,
    world_y = 200,
}

Bob.equip(Bob, "feet", LeatherBoots)

Bob.spritesheet:add_animation("walk_up", 0, 3, {250, 250, 250, 250})
Bob.spritesheet:add_animation("walk_left", 0, 3, {250, 250, 250, 250})
Bob.spritesheet:add_animation("walk_down", 4, 7, {250, 250, 250, 250})
Bob.spritesheet:add_animation("walk_right", 4, 7, {250, 250, 250, 250})
local temp = Bob.add_skill(Bob, "BasicAttack")
--Characters["Bob"]["active_skills"][1] = "BasicAttack"

Character:new{
    name = "TargetDummy",
    hp = 1000,
    movement = 0,
    spritesheet = {
        path = "resources/target_dummy.png",
        count = 1;
        width = 32,
        height = 32
    },
    world_x = 100,
    world_y = 100
}



 print("Characters:", dump(Characters))