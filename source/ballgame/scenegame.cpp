// This include: 
#include "scenegame.h"

// Local includes: 
#include "renderer.h"
#include "input.h"
#include "ball.h"
#include "player.h"
#include "logmanager.h"

// Library includes: 
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include "imgui/imgui_impl_sdl.h"

SceneGame::SceneGame()
	: m_iScore(0)
	, m_iBadSpawnCount(1)
	, m_iLives(10)
{

}

SceneGame::~SceneGame()
{
	
}

bool
SceneGame::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;
	m_pPlayer = new Player();
	m_pPlayer->Initialise(renderer);

	for (int i = 0; i < 3; ++i)
	{
		Ball* goodBall = new Ball();
		goodBall->Initialise(renderer);
		goodBall->SetColour(0, 255, 0);

		Ball* badBall = new Ball();
		badBall->Initialise(renderer);
		badBall->SetColour(255, 0, 0);

		m_goodBalls.push_back(goodBall);
		m_badBalls.push_back(badBall);
	}

	// Test logs

	Vector2 a = { 0, 0 };
	Vector2 b = { 1, 1 };
	Vector2 lerp = Vector2::Lerp(a, b, 0.5);

	char string[256];
	sprintf(string, "%f, %f", lerp.x, lerp.y);
	LogManager::GetInstance().Log(string);

	return true;
}

void
SceneGame::Process(float deltaTime, Input& input)
{
	for (std::vector<Ball*>::iterator goodBall = m_goodBalls.begin(); goodBall != m_goodBalls.end(); ++goodBall)
	{
		(*goodBall)->Process(deltaTime);
		if (IsColliding(*(*goodBall), *m_pPlayer))
		{
			++m_iScore;
			delete (*goodBall);
			(*goodBall) = AddBall(true);

			for (int i = 0; i < m_iBadSpawnCount; ++i)
			{
				m_badBalls.push_back(AddBall(false));
			}
		}
	}
	for (std::vector<Ball*>::iterator badBall = m_badBalls.begin(); badBall != m_badBalls.end(); ++badBall)
	{
		(*badBall)->Process(deltaTime);
		if (IsColliding(*(*badBall), *m_pPlayer))
		{
			--m_iLives;
			delete (*badBall);
			(*badBall) = AddBall(false);
		}
	}

	m_pPlayer->Process(deltaTime, input);
}

Ball* 
SceneGame::AddBall(bool good)
{
	Ball* ball = new Ball();
	ball->Initialise(*m_pRenderer);
	ball->SetColour(good ? 0 : 255, good ? 255 : 0, 0);
	return ball;
}

bool
SceneGame::IsColliding(Ball& ball, Player& player)
{
	Vector2 diff = ball.Position() - player.Position();
	float distance = diff.Length();

	return distance < ball.GetRadius() + player.GetRadius();
}

void
SceneGame::Draw(Renderer& renderer)
{
	std::vector<Ball*>::iterator goodBall;
	std::vector<Ball*>::iterator badBall;

	for (goodBall = m_goodBalls.begin(); goodBall != m_goodBalls.end(); ++goodBall)
	{
		(*goodBall)->Draw(renderer);
	}
	for (badBall = m_badBalls.begin(); badBall != m_badBalls.end(); ++badBall)
	{
		(*badBall)->Draw(renderer);
	}

	m_pPlayer->Draw(renderer);
}

void SceneGame::DebugDraw()
{
	ImGui::Text("Scene: Game");
	ImGui::Text("Score: %d", m_iScore);
	ImGui::SliderInt("Lives", &m_iLives, 1, 100);
	ImGui::SliderInt("Bad Spawn Count", &m_iBadSpawnCount, 1, 5);
	ImGui::Spacing();
	m_pPlayer->DebugDraw();
}