#include "Application.hpp"

namespace Graphics
{
    Application::Application()
        : Application(
            DEFAULT_WINDOW_NAME,
            Maths::Rectangle(
                DEFAULT_WINDOW_POSITION_X,
                DEFAULT_WINDOW_POSITION_Y, 
                DEFAULT_WINDOW_WIDTH, 
                DEFAULT_WINDOW_HEIGHT
            )
        )
    {
        
    }

    Application::Application(const std::string& name)
        : Application(
            name, 
            Maths::Rectangle(
                DEFAULT_WINDOW_POSITION_X, 
                DEFAULT_WINDOW_POSITION_Y, 
                DEFAULT_WINDOW_WIDTH, 
                DEFAULT_WINDOW_HEIGHT
            )
        )
    {

    }

    Application::Application(const std::string& name, float width, float height)
        :Application(
            name,
            Maths::Rectangle(
                DEFAULT_WINDOW_POSITION_X,
                DEFAULT_WINDOW_POSITION_Y,
                width,
                height
            )
        )    
    {

    }
    Application::Application(const std::string& name, float x, float y, float width, float height)
        :Application(
            name,
            Maths::Rectangle(
                x,
                y,
                width,
                height
            )
        )    
    {

    }

    Application::Application(const std::string& name, const Maths::Rectangle& bounds)
    {
        // s_Instance = this;

        m_Window = new Window(name, bounds);
        m_SoundEngine = new SoundEngine();
    }

    Application::~Application()
    {

    }

    void Application::AddLayer2D(Layer2D* layer)
    {
        m_LayerStack.push_back(layer);
        layer->Init();
    }

    Layer2D* Application::PopLayer2D()
    {
        Layer2D* layer = m_LayerStack.back();
        m_LayerStack.pop_back();
        return layer;
    }

    Layer2D* Application::PopLayer2D(Layer2D* layer)
    {
        for (int i = 0; i < m_LayerStack.size(); i++)
        {
            if (m_LayerStack[i] == layer)
            {
                m_LayerStack.erase(m_LayerStack.begin() + i);
                break;
            }
        }
        return layer;
    }

    void Application::Start()
    {
        Run();
        m_Window->Start();
    }

    void Application::Stop()
    {

    }

    void Application::OnTick()
    {
        for (Layer2D* layer : m_LayerStack)
            layer->OnTick();
    }

    void Application::OnUpdate()
    {
        for (Layer2D* layer : m_LayerStack)
            layer->OnUpdate();
    }

    void Application::OnRender()
    {
        for (Layer2D* layer : m_LayerStack)
            layer->OnRender();
    }
}
