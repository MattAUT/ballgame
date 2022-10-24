// This include: 
#include "sceneballs.h"

// Local includes: 
#include "renderer.h"
#include "input.h"
#include "ball.h"

// Library includes: 
#include <cassert>
#include <cstdlib>
#include "imgui/imgui_impl_sdl.h"

SceneBalls::SceneBalls()
	: m_pBalls{ 0 }
	, m_iShowCount(0)
{
	m_pBalls = new Ball*[100];
}
SceneBalls::~SceneBalls()
{
	
	for (int k = 0; k < 100; ++k)
	{
		delete m_pBalls[k];
		m_pBalls[k] = 0;
	}

	delete[] m_pBalls;
}

bool
SceneBalls::Initialise(Renderer& renderer)
{
	for (int k = 0; k < 100; ++k)
	{
		m_pBalls[k] = new Ball();
		m_pBalls[k]->Initialise(renderer);
		m_pBalls[k]->RandomiseColour();
	}
	m_iShowCount = 100;
	return true;
}
void
SceneBalls::Process(float deltaTime, Input& input)
{
	for (int k = 0; k < m_iShowCount; ++k)
	{
		m_pBalls[k]->Process(deltaTime);
	}
}

void
SceneBalls::Draw(Renderer& renderer)
{
	for (int k = 0; k < m_iShowCount; ++k)
	{
		m_pBalls[k]->Draw(renderer);
	}
}

void SceneBalls::DebugDraw()
{
	ImGui::Text("Scene: Bouncing Balls");
	ImGui::SliderInt("Show Count", &m_iShowCount, 1, 100);
	static int editBallNumber = 0;
	ImGui::SliderInt("Edit ball", &editBallNumber, 0, 99);
	m_pBalls[editBallNumber]->DebugDraw();
}