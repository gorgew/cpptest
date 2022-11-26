Skills = {}

Skill = {
    magnitude = 1,
    magnitudeRangeLow = 0,
    magnitudeRangeHigh = 0,
    delayTime = 0,
    rangeMagnitude = 0
}

SkillMeta = { __index = Skill }


function Skill:run()
    f1()
end

ActiveSkills = {}

ActiveSkill = {
    isAttack = false,
    targetSelf = false,
    targetAlly = false,
    targetEnemy = true,
    targetTile = false,
    targetEnvironment = false,
    rangeCollideEnvironment = false,
    rangeCollideEnemy = false,
    rangeMagnitude = 1
}

ActiveSkill.__index = ActiveSkill
setmetatable(ActiveSkill, Skill)

--[[
    isAttack: modified empowered attack
    // how to know which animation?
    execute(caster, targets, ...)
    

--]]
function ActiveSkill:new(args)
    if (args.name == nil) then print("Skill is unnamed!\n") end
    ActiveSkills[args.name] = setmetatable({
        isAttack = args.isAttack,
        targetSelf = args.targetSelf,
        targetAlly = args.targetAlly,
        targetEnemy = args.targetEnemy,
        targetEnvironment = args.targetEnvironment,
        rangeCollideEnvironment = args.rangeCollideEnvironment,
        rangeCollideEnemy = args.rangeCollideEnemy,
        magnitude = args.magnitude,
        magnitudeRangeLow = args.magnitudeRangeLow,
        magnitudeRangeHigh = args.magnitudeRangeHigh

    }, ActiveSkill)
end

function ActiveSkill:run()
end

PassiveSkills = {}

PassiveSkill = {
    onTurnStart = false,
    onTurnEnd = false,
    onMove = false,
    onPreAttack = false,
    onPostAttack = false,
    onPreDefense = false,
    onPostDefense = false,
    onEffectTick = false,
    onKill = false,
    onDeath = false
}

PassiveSkillMeta = { __index = PassiveSkill }

function PassiveSkill:new(args)
    if (args.name == nil) then print("Skill is unnamed!\n") end
    PassiveSkills[args.name] = setmetatable({
        onTurnStart = args.onTurnStart,
        onTurnEnd = args.onTurnEnd,
        onMove = args.onMove,
        onPreAttack = args.onPreAttack,
        onPostAttack = args.onPostAttack,
        onPreDefense = args.onPreDefense,
        onPostDefense = args.onPostDefense,
        onEffectTick = args.onEffectTick,
        onKill = args.onKill,
        onDeath = args.onDeath
    }, PassiveSkillMeta)
    return args.name
end

function PassiveSkill:run()
end


--IDK JUST A TEST ON WHAT MY API to LOOK LIKE
 
BasicAttack = ActiveSkill:new({
    name = 'BasicAttack',
    isAttack = true
})
--[[
Fireball = ActiveSkill:new({
    targetCollideEnemy = true,
    targetCollideEnvironment = true,
    rangeMagnitude = 5,
    skill = "FireballSkill"
})

FireballSkill = Skill:new({
    magnitude = 2,
})

function FireballSkill:run()
    --deal magnitude damage to 
end
]]