#pragma once

#include <vector>

#include "Camera.hpp"
#include "Entity.hpp"
#include "Renderer2D.hpp"

namespace Graphics
{
    class Scene3D
    {
        private:
            std::vector<Entity*> m_Entities;
            Camera* m_Camera;

        public:
            Scene3D();
            Scene3D(Camera* camera);

            ~Scene3D();

            void Add(Entity* entity);
            void SetCamera(Camera* camera);

            void Update();
            void Render();

            Camera* GetCamera() const;
            const std::vector<Entity*>& GetEntities() const;
            

    };
};