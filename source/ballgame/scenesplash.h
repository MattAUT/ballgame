#ifndef __SCENESPLASH_H__
#define __SCENESPLASH_H__

#include "scene.h"

// Forward Declarations
class Game;
class Sprite;

class SceneSplash : public Scene
{
	// Member Methods
public:
	SceneSplash(int nextSceneIndex, Game& game);
	virtual ~SceneSplash();

	virtual bool Initialise(Renderer& renderer) override;
	virtual void Process(float deltaTime, Input& input) override;
	virtual void Draw(Renderer& renderer) override;
	virtual void DebugDraw() override;
	void End();

protected:
private:
	SceneSplash(const SceneSplash& sceneSplah);
	SceneSplash& operator=(const SceneSplash& sceneSplash);

	// Member Data
public:
protected:
	Game& m_rGame;
	Sprite* m_pLogo;
	Sprite* m_pText;
	int m_iNextScene;

	float m_fTime; 
	float m_fSceneLength; // Amount of time the splash screen will last
private:

};

#endif // __SCENESPLASH_H__

