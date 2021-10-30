#include "Layer2D.hpp"

namespace Graphics
{
    Layer2D::Layer2D(const Maths::mat4& projectionMatrix)
    {
        m_Renderer = new Renderer2D(projectionMatrix);
    }

    void Layer2D::Init()
    {

    }

    void Layer2D::OnInit(Renderer2D& renderer)
    {
        
    }

    void Layer2D::Submit(IRenderable2D* renderable)
    {
        this->m_SubmittedRenderables.push_back(renderable);
    }

    void Layer2D::OnRender()
    {
        for (const IRenderable2D* renderable : m_SubmittedRenderables)
            renderable->Submit(m_Renderer);

        m_SubmittedRenderables.clear();
    }

    void Layer2D::OnRender(Renderer2D& renderer)
    {

    }

    void Layer2D::OnUpdateInternal()
    {

    }
}