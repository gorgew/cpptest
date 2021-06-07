#include <catch2/catch.hpp>

#include <entt/entity/registry.hpp>
#include <WorldCacheSystem.hpp>

TEST_CASE("WorldCacheSystem construction", "=") {
    unsigned int x = 5;
    unsigned int y = 4;
    unsigned int z = 3;
    entt::registry registry;
    WorldCacheSystem wcs {registry, x, y, z};
    REQUIRE(wcs.terrain_map.size() == x);
    REQUIRE(wcs.environment_map.size() == x);
    REQUIRE(wcs.character_map.size() == x);

    for (unsigned int i = 0; i < y; ++i) {
        REQUIRE(wcs.terrain_map[i].size() == y);
        REQUIRE(wcs.environment_map[i].size() == y);
        REQUIRE(wcs.character_map[i].size() == y);

        for (unsigned int j = 0; j < z; ++j) {
            REQUIRE(wcs.terrain_map[i][j].size() == z);
            REQUIRE(wcs.environment_map[i][j].size() == z);
            REQUIRE(wcs.character_map[i][j].size() == z);
        }
    }
}

TEST_CASE("WorldCacheSystem: load/observe",  "=") {
    unsigned int x = 2;
    unsigned int y = 2;
    unsigned int z = 3;
    
    entt::registry registry;

    auto environment_1 = registry.create();
    auto environment_2 = registry.create();
    registry.emplace<environment>(environment_1);
    registry.emplace<world_pos>(environment_1, 0, 0, 0);
    registry.emplace<environment>(environment_2);
    registry.emplace<world_pos>(environment_2, 1, 1, 0);
    auto terrain_1 = registry.create();
    auto terrain_2 = registry.create();
    registry.emplace<terrain>(terrain_1);
    registry.emplace<world_pos>(terrain_1, 0, 0, 0);
    registry.emplace<terrain>(terrain_2);
    registry.emplace<world_pos>(terrain_2, 1, 1, 0);
    auto character_1 = registry.create();
    auto character_2 = registry.create();
    registry.emplace<character>(character_1);
    registry.emplace<world_pos>(character_1, 0, 0, 0);
    registry.emplace<character>(character_2);
    registry.emplace<world_pos>(character_2, 1, 1, 0);
    WorldCacheSystem wcs {registry, x, y, z};
    REQUIRE(wcs.environment_map[0][0][0] == environment_1);
    REQUIRE(wcs.environment_map[1][1][0] == environment_2);
    REQUIRE(wcs.terrain_map[0][0][0] == terrain_1);
    REQUIRE(wcs.terrain_map[1][1][0] == terrain_2);
    REQUIRE(wcs.character_map[0][0][0] == character_1);
    REQUIRE(wcs.character_map[1][1][0] == character_2);

    registry.emplace_or_replace<old_world_pos>(character_1, 0, 0, 0);
    registry.emplace_or_replace<world_pos>(character_1, 1, 0, 0);
    registry.emplace_or_replace<old_world_pos>(character_2, 1, 1, 0);
    registry.emplace_or_replace<world_pos>(character_2, 0, 1, 0);
    wcs.observe(registry);
    
    REQUIRE(wcs.character_map[0][1][0] == character_2);
    REQUIRE(wcs.character_map[1][0][0] == character_1);
    REQUIRE(registry.all_of<world_pos, character>(character_1));
    registry.destroy(character_1);
    REQUIRE(wcs.character_map[1][0][0] == entt::null);
}