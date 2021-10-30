#pragma once

#include "Camera.hpp"
#include "Entity.hpp"
#include "Renderer2D.hpp"

#include <vector>

namespace Graphics
{
    class Scene2D
    {
        private:
            std::vector<Entity*> m_Entities;
            Camera* m_Camera;

        public:
            Scene2D();
            Scene2D(Camera* camera);

            ~Scene2D();

            void Add(Entity* entity);
            void SetCamera(Camera* camera);

            void Update();
            void Render(Renderer2D& renderer);

            Camera* GetCamera() const;
            const std::vector<Entity*>& GetEntities() const;

    };
};