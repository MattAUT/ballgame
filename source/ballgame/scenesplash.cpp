// COMP710 GP Framework 2022

#include "scenesplash.h"

#include "renderer.h"
#include "sprite.h"
#include "game.h"
#include "input.h"

#include <cassert>
#include <cmath>
#include "imgui/imgui_impl_sdl.h"

SceneSplash::SceneSplash(int nextSceneIndex, Game& game)
	: m_pLogo(0)
	, m_fTime(-0.4f) 
	, m_fSceneLength(5)
	, m_iNextScene(nextSceneIndex)
	, m_rGame(game)
{

}

SceneSplash::~SceneSplash()
{
	m_fSceneLength = 0.0f;

	delete m_pLogo;
	m_pLogo = NULL;

	delete m_pText;
	m_pText = NULL;
}

bool SceneSplash::Initialise(Renderer& renderer)
{
	m_pLogo = renderer.CreateSprite("assets\\splash-logo.png");

	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	m_pLogo->SetX(SCREEN_WIDTH / 2);
	m_pLogo->SetY((SCREEN_HEIGHT / 2) - 50);
	m_pLogo->SetScale(0.4f);
	m_pLogo->SetAlpha(0.0f);

	const char* text = "COMP710 GAME PROGRAMMING";

	renderer.CreateStaticText(text, 32);
	m_pText = renderer.CreateSprite(text);
	m_pText->SetX(SCREEN_WIDTH / 2);
	m_pText->SetY((SCREEN_HEIGHT / 2) + 100);

	return true;
}

void SceneSplash::End()
{
	m_fTime = 0;
	m_rGame.SwitchScenes(m_iNextScene);
}

void SceneSplash::Process(float deltaTime, Input& input)
{
	m_fTime += deltaTime;
	float alpha = powf(sinf((M_PI * m_fTime) / m_fSceneLength), 0.65);
	m_pLogo->SetAlpha(alpha);

	if (m_fTime > m_fSceneLength + 0.2f)
	{
		End();
	}
}

void SceneSplash::Draw(Renderer& renderer)
{
	m_pLogo->Draw(renderer);
	m_pText->Draw(renderer);
}

void SceneSplash::DebugDraw()
{
	ImGui::Text("Scene: Splash Screen");
}