#include "Scene3D.hpp"

namespace Graphics
{
    Scene3D::Scene3D()
    {

    }

    Scene3D::Scene3D(Camera* camera): m_Camera(camera)
    {

    }

    void Scene3D::Add(Entity* entity)
    {
        m_Entities.push_back(entity);
    }

    void Scene3D::SetCamera(Camera* camera)
    {
        m_Camera = camera;
    }

    void Scene3D::Update()
    {

    }

    void Scene3D::Render()
    {

    }

    Camera* Scene3D::GetCamera() const
    {
        return m_Camera;
    }

    const std::vector<Entity*>& Scene3D::GetEntities() const
    {
        return m_Entities;
    }
};