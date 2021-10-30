#pragma once

#include "Camera.hpp"

namespace Graphics
{
    class OrthoCamera : public Camera
    {
        private:

        public:
            OrthoCamera(const Maths::mat4& projectionMatrix);
            ~OrthoCamera();

            void Focus() override;
            void Update() override;
    };
};