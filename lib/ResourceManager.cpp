#include "ResourceManager.hpp"
#include <fmt/printf.h>

void ResourceManager::load_character_resources(sol::state& lua, std::shared_ptr<Injector> injector) {

    
    sol::table characters = lua["characters"];
    
    for (int i = 1; i <= characters.size(); i++) {
        std::string char_name = characters[i]["name"];
        std::string pic_name = char_name + "_profile";

        injector->tex_man.add_texture(pic_name,
            "../resources/sprites/" + pic_name + ".png");
        profile_pics[char_name] = nk_image_id(static_cast<int>(injector->tex_man.get_id(pic_name)));
    }

}


struct nk_image ResourceManager::get_profile_pic(std::string name) {

    if (profile_pics.contains(name)) {
        return profile_pics[name];
    }
    else {
        fmt::print("Couldn't find {}'s profile pic\n", name);
        return profile_pics["Default"];
    }
}
