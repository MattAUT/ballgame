#ifndef __SCENEGAME_H__
#define __SCENEGAME_H__

#include "scene.h"

#include <vector>

// Forward Declarations
class Ball;
class Player;

class SceneGame : public Scene
{
	// Member Methods
public:
	SceneGame();
	virtual ~SceneGame();

	virtual bool Initialise(Renderer& renderer) override;
	virtual void Process(float deltaTime, Input& input) override;
	virtual void Draw(Renderer& renderer) override;
	virtual void DebugDraw() override;

protected:
	Ball* AddBall(bool good);
	bool IsColliding(Ball& ball, Player& player);
private:
	SceneGame(const SceneGame& sceneGame);
	SceneGame &operator=(const SceneGame& sceneGame);

	// Member Data
public:
protected:
	Renderer* m_pRenderer;
	Player* m_pPlayer;
	std::vector<Ball*> m_goodBalls;
	std::vector<Ball*> m_badBalls;

	int m_iScore;
	int m_iBadSpawnCount;
	int m_iLives;

private:

};

#endif // __SCENEGAME_H__

