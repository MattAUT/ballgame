// COMP710 GP Framework 2022

// This include:
#include "game.h"

// Library includes:
#include "renderer.h"
#include "logmanager.h"
#include "scene.h"
#include "input.h"
#include "xinputcontroller.h"

#include "scenesplash.h"
#include "sceneballs.h"
#include "scenegame.h"

#include <cstdio>
#include "imgui/imgui_impl_sdl.h"

// Static Members:
Game* Game::sm_pInstance = 0;

Game& 
Game::GetInstance()
{
    if (sm_pInstance == 0)
    {
        sm_pInstance = new Game();
    }

    return (*sm_pInstance);
}

void 
Game::DestroyInstance()
{
    delete sm_pInstance;
    sm_pInstance = 0;
}

Game::Game()
	: m_pRenderer(0)
	, m_bLooping(true)
	, m_iCurrentScene(0)
	, m_fExecutionTime(0)
	, m_fElapsedSeconds(0)
	, m_iFrameCount(0)
	, m_fFrametimeUpdate(0.25f)
	, m_bShowDebugWindow(true)
	, m_bPaused(false)

{

}

Game::~Game()
{
    delete m_pRenderer;
	delete m_pInput;

    m_pRenderer = 0;
	m_pInput = NULL;
}

void
Game::Quit()
{
    m_bLooping = false;
}

void
Game::SwitchScenes(int index)
{
	m_iCurrentScene = index;
}

void
Game::ToggleDebugWindow()
{
	m_bShowDebugWindow = !m_bShowDebugWindow;
	m_pInput->ShowMouseCursor(m_bShowDebugWindow);
}

bool
Game::Initialise()
{
    int bbWidth = 1024;
    int bbHeight = 768;

    m_pRenderer = new Renderer();
    if (!m_pRenderer->Initialise(true, bbWidth, bbHeight))
    {
        LogManager::GetInstance().Log("Renderer failed to initialise!");
        return false;
    }

    bbWidth = m_pRenderer->GetWidth();
    bbHeight = m_pRenderer->GetHeight();

    m_iLastTime = SDL_GetPerformanceCounter();

    m_pRenderer->SetClearColour(0, 0, 0);

	m_pInput = new Input();
	m_pInput->Initialise();

	Scene* sceneSplash = new SceneSplash(1, *this);
	Scene* sceneGame = new SceneGame();
	Scene* sceneBalls = new SceneBalls();

	sceneSplash->Initialise(*m_pRenderer);
	sceneGame->Initialise(*m_pRenderer);
	sceneBalls->Initialise(*m_pRenderer);
	m_scenes.push_back(sceneSplash);
	m_scenes.push_back(sceneGame);
	m_scenes.push_back(sceneBalls);

    return true;
}

bool
Game::DoGameLoop()
{
    const float stepSize = 1.0f / 60.0f;

	m_pInput->ProcessInput();
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplSDL2_ProcessEvent(&event);
    }

	if (event.window.event == SDL_WINDOWEVENT_CLOSE)
	{
		Quit();
	}

    if (m_bLooping)
    {
        Uint64 current = SDL_GetPerformanceCounter();
        float deltaTime = (current - m_iLastTime) / static_cast<float>(SDL_GetPerformanceFrequency());
        m_iLastTime = current;

        m_fExecutionTime += deltaTime;

        Process(deltaTime);

#ifdef USE_LAG
        m_fLag += deltaTime;

        int innerLag = 0;

        while (m_fLag >= stepSize)
        {
            Process(stepSize);

            m_fLag -= stepSize;

            ++m_iUpdateCount;
            ++innerLag;
        }
#endif //USE_LAG

        Draw(*m_pRenderer);
    }

    return m_bLooping;
}

void 
Game::Process(float deltaTime)
{
	XInputController* controller = m_pInput->GetController(0);
	bool debugKeyPressed = 
		m_pInput->GetKeyState(SDL_SCANCODE_GRAVE) == BS_PRESSED || 
		controller->GetButtonState(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BS_PRESSED;
	bool pauseKeyPressed = 
		m_pInput->GetKeyState(SDL_SCANCODE_ESCAPE) == BS_PRESSED || 
		controller->GetButtonState(SDL_CONTROLLER_BUTTON_START) == BS_PRESSED;

	if (debugKeyPressed)
	{
		ToggleDebugWindow();
	}

	if (pauseKeyPressed)
	{
		m_bPaused = !m_bPaused;
	}

	if (!m_bPaused)
	{
		ProcessFrameCounting(deltaTime);
		m_scenes[m_iCurrentScene]->Process(deltaTime, *m_pInput);
	}
}

void
Game::Draw(Renderer& renderer)
{
    ++m_iFrameCount;

    renderer.Clear();

	m_scenes[m_iCurrentScene]->Draw(renderer);

	DebugDraw();

    renderer.Present();
}

void
Game::DebugDraw()
{
	if (m_bShowDebugWindow)
	{
		bool open = true;
		ImGui::Begin("Debug Window", &open, ImGuiWindowFlags_MenuBar);
		ImGui::Text("COMP710 GP Framework (%s)", "2022, S2");
		if (ImGui::Button("Quit"))
		{
			Quit();
		}
		ImGui::SliderInt("Active scene", &m_iCurrentScene, 0, m_scenes.size() - 1, "%d");
		m_scenes[m_iCurrentScene]->DebugDraw();
		ImGui::End();

		ImGui::Begin("Statistics", &open, ImGuiWindowFlags_MenuBar);
		char* framerateText = new char[30];
		sprintf(framerateText, "Framerate: %dFPS", m_iFPS);
		ImGui::Text(framerateText);
		char* frametimeText = new char[30];
		sprintf(frametimeText, "Frame Time: %fms", 1.0f / m_iFPS);
		ImGui::Text(frametimeText);
		ImGui::End();
	}
}

void 
Game::ProcessFrameCounting(float deltaTime)
{
    // Count total simulation time elapsed:
    m_fElapsedSeconds += deltaTime;

    // Frame Counter:
    if (m_fElapsedSeconds > m_fFrametimeUpdate)
    {
        m_fElapsedSeconds -= m_fFrametimeUpdate;
        m_iFPS = m_iFrameCount / m_fFrametimeUpdate;
        m_iFrameCount = 0;
    }
}
