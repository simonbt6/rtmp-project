#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-24 
 * 
 * 
 */
#include "ILayer.hpp"
#include "Renderer2D.hpp"
#include "TextRenderer.hpp"
#include "IRenderable2D.hpp"

#include <maths/maths.hpp>


#include <inttypes.h>
#include <vector>

namespace Graphics
{
    class Layer2D : public Layer
    {
        private:
            Renderer2D* m_Renderer;
            

        protected:
            std::vector<IRenderable2D*> m_SubmittedRenderables;

        public:
            Layer2D(const Maths::mat4& projectionMatrix);

            virtual ~Layer2D(){};

            virtual void Init();
            virtual void OnInit(Renderer2D& renderer);

            virtual void Submit(IRenderable2D* renderable);

            void OnUpdateInternal() override;
            virtual void OnRender(Renderer2D& renderer);
            void OnRender() override;

        protected:
            // bool OnResize(uint32_t width, uint32_t height) override;

    };
}