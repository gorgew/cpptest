#include <robin_hood.h>

#include <glad/glad.h>
#include <string>
#include <vector>
/** 
 * @brief Compiles and manages shaders. Also creates and manages shader programs.
 */
class ShaderManager {
    robin_hood::unordered_map<const std::string, GLuint> name_to_shader_map;
    robin_hood::unordered_map<const std::string, GLuint> name_to_program_map;

    bool check_shader(GLuint shader_id);

    public:
        void add_shader(const std::string name, const std::string filepath);

        void add_program(const std::string name, ...);
};