Skills = {}

Skill = {
    magnitude = 1,
    magnitudeRangeLow = 0,
    magnitudeRangeHigh = 0
    delayTime = 0
    rangeMagnitude = 0
}

SkillMeta = { __index = Skill }


function Skill:act()
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

ActiveSkillMeta = { __index = ActiveSkill }

function ActiveSkill:new(args)
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
        magnitudeRangeHigh args.magnitudeRangeHigh
    }, ActiveSkillMeta)
end

function ActiveSkill:act()
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
end

function PassiveSkill:act()
end


--IDK JUST A TEST ON WHAT MY API to LOOK LIKE

BasicAttack = ActiveSkill:new({
    isAttack = true
})

Fireball = ActiveSkill:new({
    targetCollideEnemy = true,
    targetCollideEnvironment = true,
    rangeMagnitude = 5
    skill = "FireballSkill"
})

FireballSkill = Skill:new({
    magnitude = 2,
})

function FireballSkill:act()
    --deal magnitude damage to 
end
