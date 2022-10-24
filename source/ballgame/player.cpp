// COMP710 - Matthew Tribble
// Player

#include <cassert>

#include "player.h"
#include "renderer.h"
#include "sprite.h"
#include "input.h"
#include "xinputcontroller.h"

#include <SDL.h>
#include "imgui/imgui_impl_sdl.h"
#include <limits>

// Constructor
Player::Player()
	: m_movementInput{ 0, 0 }
	, m_fDeadzone(0.08f)
	, m_fSpeed(300)
	, m_fAccelleration(25)
{

}

// Destructor
Player::~Player()
{

}

bool
Player::Initialise(Renderer& renderer)
{
	m_pSprite = renderer.CreateSprite("assets\\sprites\\ball.png");
	m_xMax = renderer.GetWidth();
	m_yMax = renderer.GetHeight();

	m_position.x = renderer.GetWidth() / 2.0f;
	m_position.y = renderer.GetHeight() / 2.0f;
	m_velocity.x = 0;
	m_velocity.y = 0;

	m_pSprite->SetPosition(m_position);
	m_fRadius = m_pSprite->GetWidth() / 2.0f;

    return true;
}

Vector2
Player::GetMovementInput(Input& input, const Vector2& previousInput)
{
	// The XInput API returns an 16 bit signed integer for stick values, so we divide 
	// by that limit to get an input value between -1 and 1
	Vector2 stickInputVector = input.GetController(0)->GetLeftStick() / static_cast<float>(std::numeric_limits<int16_t>::max());
	
	if (stickInputVector.x < m_fDeadzone && stickInputVector.x > -m_fDeadzone)
	{
		stickInputVector.x = 0;
	}
	if (stickInputVector.y < m_fDeadzone && stickInputVector.y > -m_fDeadzone)
	{
		stickInputVector.y = 0;
	}


	// This is bad, but it'll do for now
	// TODO: do better keyboard input for actual games
	Vector2 keyInputVector = 
	{ 
		static_cast<float>(input.GetKeyState(SDL_SCANCODE_D) == BS_HELD ? 1 : input.GetKeyState(SDL_SCANCODE_A) == BS_HELD ? -1 : 0),
		static_cast<float>(input.GetKeyState(SDL_SCANCODE_S) == BS_HELD ? 1 : input.GetKeyState(SDL_SCANCODE_W) == BS_HELD ? -1 : 0)
	};

	if (keyInputVector.LengthSquared() > stickInputVector.LengthSquared())
	{
		return keyInputVector;
	}

	return stickInputVector;
}

void
Player::Process(float deltaTime, Input& input)
{
	m_movementInput = GetMovementInput(input, m_movementInput) * m_fSpeed;
	m_velocity = Vector2::Lerp(m_velocity, m_movementInput, m_fAccelleration * deltaTime) ;

	Vector2 oldPosition = m_position;
	m_position += m_velocity * deltaTime;

	if (m_position.x + m_fRadius > m_xMax || m_position.x - m_fRadius < 0)
	{
		m_position.x = oldPosition.x;
	}

	if (m_position.y + m_fRadius > m_yMax || m_position.y - m_fRadius < 0)
	{
		m_position.y = oldPosition.y;
	}

	m_pSprite->SetPosition(m_position);
}

Vector2&
Player::Position()
{
	return m_position;
}

float Player::GetRadius() const
{
	return static_cast<float>(m_pSprite->GetWidth()) / 2.f;
}

void
Player::Draw(Renderer& renderer)
{
	m_pSprite->Draw(renderer);
}

void Player::DebugDraw()
{
	ImGui::Text("Player");
	ImGui::SliderFloat("Speed", &m_fSpeed, 100, 1000);
	ImGui::SliderFloat("Acceleration", &m_fAccelleration, 10, 40);
	ImGui::Text("Movement Input (%f, %f)", m_movementInput.x, m_movementInput.y);
	ImGui::Text("Velocity (%f, %f)", m_velocity.x, m_velocity.y);
}
