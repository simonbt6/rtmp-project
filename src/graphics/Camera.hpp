#pragma once

#include <maths/maths.hpp>

namespace Graphics
{
    class Camera
    {
        protected:
            Maths::mat4 m_ProjectionMatrix, m_ViewMatrix;
            Maths::vec3 m_Position, m_Rotation, m_FocalPoint;

        public:
            Camera(const Maths::mat4& projectionMatrix);

            virtual ~Camera();

            virtual void Focus();
            virtual void Update();

            const Maths::vec3& GetPosition() const;
            void SetPosition(const Maths::vec3& position);

            const Maths::vec3& GetRotation() const;
            void SetRotation(const Maths::vec3& rotation);


            void Translate(const Maths::vec3& translation);
            void Rotate(const Maths::vec3& rotation);

            void Translate(float x, float y, float z);
            void Rotate(float x, float y, float z);

            const Maths::vec3& GetFocalPoint() const;
            const Maths::mat4& GetViewMatrix() const;
            const Maths::mat4& GetProjectionMatrix() const;

            void SetProjectionMatrix(const Maths::mat4& projectionMatrix);
            
    };
};
