#include <PhysicsComponents.hpp>

#include "FontBuilder.hpp"
#include FT_FREETYPE_H
#include <fmt/format.h>
#include <cstdlib>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

FontBuilder::FontBuilder(std::shared_ptr<Injector> injector) {

    this->injector = injector;
    
    if (FT_Init_FreeType(&ft_lib)) {
        fmt::print("Freetype loading failure\n");
        std::exit(1);
    }

    injector->shader_man.add_shader("fonts-v", "../resources/fonts.vert", GL_VERTEX_SHADER);
    injector->shader_man.add_shader("fonts-f", "../resources/fonts.frag", GL_FRAGMENT_SHADER);
    injector->shader_man.add_program("fonts", {"fonts-v", "fonts-f"});
    program_id = injector->shader_man.get_program_id("fonts");
    glUseProgram(program_id);
    fmt::print("Font programid: {}\n", program_id);
    //glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(injector->config.width), 0.0f, static_cast<float>(injector->config.height));
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    fmt::print("uniform location: {}\n", glGetUniformLocation(program_id, "model"));
    glUniformMatrix4fv(glGetUniformLocation(program_id, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(program_id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(program_id, "view"), 1, GL_FALSE, glm::value_ptr(view));

    glUniform2f(glGetUniformLocation(program_id, "size"), 100.0f, 100.0f);
    glUniform3f(glGetUniformLocation(program_id, "color"), 0.604, 0.804, 0.196);
    injector->vert_man.add_rect_flipped("square", 1.0f, 1.0f);
    char_vert_id = injector->vert_man.get_array_id("square");
}

void FontBuilder::add_font(std::string font_name, const char* file_path, unsigned int height) {

    FT_Face face; 
    if (FT_New_Face(ft_lib, file_path, 0, &face)) {
        fmt::print("Invalid font path \"{}\"\n", file_path);
        std::exit(1);
    }

    FT_Set_Pixel_Sizes(face, 0, height);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned int max_height = 0;
    for (unsigned char c = 0; c < 128; ++c) {
        
         if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            
            fmt::print("Failed to load char {} in font\n", c, font_name);
        }

        std::string tex_name = font_name + "-" + std::to_string(height) + '-' + (char) c; 
        
        injector->tex_man.add_font_texture(tex_name, face);
        /*
        fmt::print("char: {} width: {}, height: {}, bearing_x: {}, bearing_y: {}, advance: {}\n", 
            c, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap_left, face->glyph->bitmap_top, (face->glyph->advance.x / 64));
        fmt::print("tex_name: {} | tex_id: {}\n", tex_name, injector->tex_man.get_id(tex_name));
        */
       
       auto glyph_height = face->glyph->bitmap.rows;
       if (glyph_height > max_height) {
           max_height = glyph_height;
       }

        char_frame_data data = {
            {
                char_vert_id,
                20,
                GL_TRIANGLE_FAN,
                injector->tex_man.get_id(tex_name),
                program_id
            },
            c,
            glm::ivec3(1.0),
            face->glyph->bitmap.width, 
            glyph_height,
            face->glyph->bitmap_left,
            face->glyph->bitmap_top,
            (face->glyph->advance.x / 64)
        };
        auto key_tuple = std::make_tuple(font_name, height, c);
        font_char_to_data_map[key_tuple] = data;
        auto key_pair = std::pair(font_name, height);
        font_to_data_map[key_pair] = {max_height};
    }
    FT_Done_Face(face);
}

char_frame_data FontBuilder::get_char(std::string font_name, unsigned int height, unsigned char character) {
    auto key = std::make_tuple(font_name, height, character);
    if (font_char_to_data_map.contains(key)) {
        return font_char_to_data_map[key];
    }
    else {
        fmt::print("Char not found\n");
        std::exit(1);
    }
}

font_data FontBuilder::get_font_data(std::string font_name, unsigned int height) {
    auto key = std::make_pair(font_name, height);
    if (font_to_data_map.contains(key)) {
        return font_to_data_map[key];
    }
    else {
        fmt::print("Char not found\n");
        std::exit(1);
    }
}

void FontBuilder::add_string(entt::registry& registry, std::string str, std::string font, unsigned int height, glm::vec3 pos, glm::vec3 color) {

    auto max_height = get_font_data(font, height).max_height;

    unsigned int advance = 0;
    for (auto& c : str) {
        const auto entity = registry.create();
        auto c_data = get_char(font, height, c);
        c_data.color = color;

        registry.emplace<char_frame_data>(entity, c_data);

        float x = pos.x + c_data.bearing_x + advance + c_data.width / 2;
        float y = pos.y - c_data.height / 2 + c_data.bearing_y - max_height;
        glm::vec3 transformed_pos = glm::vec3(x, y, 0);
        registry.emplace<position>(entity, transformed_pos);
        advance += c_data.advance;
    }
}

FontBuilder::~FontBuilder() {

    FT_Done_FreeType(ft_lib);
}
