#include <glad/glad.h>
#include <string>
#include <vector>
#include <unordered_map>
/** 
 * @brief Compiles and manages shaders. Also creates and manages shader programs.
 */
class ShaderManager {
    std::unordered_map<std::string, GLuint> name_to_shader_map;
    std::unordered_map<std::string, GLuint> name_to_program_map;

    bool check_shader(GLuint shader_id);

    bool check_program(GLuint program_id);

    public:
        void add_shader(std::string name, std::string filepath, GLenum shader_type);

        void add_program(std::string name, std::initializer_list<std::string> programs);

        void delete_shaders();

        void delete_programs();

        void use(std::string name); 

        GLuint& get_program_id(std::string name);
};