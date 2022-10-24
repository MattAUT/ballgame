#ifndef __SCENEBALL_H__
#define __SCENEBALL_H__

#include "scene.h"

// Forward Declarations
class Ball;

class SceneBalls : public Scene
{
	// Member Methods
public:
	SceneBalls();
	virtual ~SceneBalls();

	virtual bool Initialise(Renderer& renderer) override;
	virtual void Process(float deltaTime, Input& input) override;
	virtual void Draw(Renderer& renderer) override;
	virtual void DebugDraw() override;

protected:
private:
	SceneBalls(const SceneBalls& sceneBalls);
	SceneBalls &operator=(const SceneBalls& sceneBalls);

	// Member Data
public:
protected:
	Ball** m_pBalls;
	int m_iShowCount;

private:

};

#endif // __SCENESPLASH_H__

