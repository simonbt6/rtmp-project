#include "Camera.hpp"

namespace Graphics
{
    Camera::Camera(const Maths::mat4& projectionMatrix)
        : m_ProjectionMatrix(projectionMatrix)
    {
        m_ViewMatrix = Maths::mat4::Identity();
        m_Position = Maths::vec3();
        m_Rotation = Maths::vec3();
    }

    Camera::~Camera()
    {

    }

    void Camera::Focus()
    {

    }

    void Camera::Update()
    {

    }

    const Maths::vec3& Camera::GetPosition() const
    {
        return m_Position;
    }

    void Camera::SetPosition(const Maths::vec3& position)
    {
        m_Position = position;
    }

    const Maths::vec3& Camera::GetRotation() const
    {
        return m_Rotation;
    }

    void Camera::SetRotation(const Maths::vec3& rotation)
    {
        m_Rotation = rotation;
    }

    const Maths::mat4& Camera::GetProjectionMatrix() const
    {
        return m_ProjectionMatrix;
    }

    void Camera::SetProjectionMatrix(const Maths::mat4& projectionMatrix)
    {
        m_ProjectionMatrix = projectionMatrix;
    }

    void Camera::Translate(const Maths::vec3& translation)
    {
        m_Position += translation;
    }

    void Camera::Rotate(const Maths::vec3& rotation)
    {
        m_Rotation += rotation;
    }

    void Camera::Translate(float x, float y, float z)
    {
        m_Position += Maths::vec3(x, y, z);
    }

    void Camera::Rotate(float x, float y, float z)
    {
        m_Rotation += Maths::vec3(x, y, z);
    }

    const Maths::vec3& Camera::GetFocalPoint() const
    {
        return m_FocalPoint;
    }

    const Maths::mat4& Camera::GetViewMatrix() const
    {
        return m_ViewMatrix;
    }
}