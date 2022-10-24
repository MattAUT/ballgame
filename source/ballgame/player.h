// COMP710 - Matthew Tribble
// Player

#ifndef __PLAYER_H__ 
#define __PLAYER_H__ 

#include "vector2.h"

// Forward Declarations:
class Renderer;
class Sprite;
class Input;

class Player
{
// Member Methods:
public:
    Player();
	 ~Player();

	 bool Initialise(Renderer& renderer);
	 void Process(float deltaTime, Input& input);
	 void Draw(Renderer& renderer);
	 void DebugDraw();

	 Vector2& Position();
	 float GetRadius() const;

protected:
	Vector2 GetMovementInput(Input& input, const Vector2& previousInput);
private:
    Player(const Player& player);
    Player& operator=(const Player& player);

// Member Data: 
public:
protected:
	Sprite* m_pSprite;
	Vector2 m_position;
	Vector2 m_velocity;

	Vector2 m_movementInput;

	float m_fRadius;
	float m_fDeadzone;
	float m_fSpeed;
	float m_fAccelleration;
	int m_xMax;
	int m_yMax;

private:
};

#endif // __PLAYER_H__
