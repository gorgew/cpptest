#include "PhysicsComponents.hpp"
#include "GraphicsComponents.hpp"

/**
 * @brief 
*/
class GraphicsSystem {

    public:
        GraphicsSystem();
        /**
         * @brief Draws the entity based on position and graphics_data components
         * @param pos Position component
         * @param graphics_data Graphics data component
        */
        void draw(pos pos, graphics_data g_data);
};