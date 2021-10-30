#include "Scene2D.hpp"

namespace Graphics
{
    Scene2D::Scene2D()
    {

    }

    Scene2D::Scene2D(Camera* camera): m_Camera(camera)
    {

    }

    void Scene2D::Add(Entity* entity)
    {
        m_Entities.push_back(entity);
    }

    void Scene2D::SetCamera(Camera* camera)
    {
        m_Camera = camera;
    }

    void Scene2D::Update()
    {

    }

    void Scene2D::Render(Renderer2D& renderer)
    {

    }

    Camera* Scene2D::GetCamera() const
    {
        return m_Camera;
    }

    const std::vector<Entity*>& Scene2D::GetEntities() const
    {
        return m_Entities;
    }
};