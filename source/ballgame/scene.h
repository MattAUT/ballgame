// COMP710 GP Framework 2022
#ifndef __SCENE_H__
#define __SCENE_H__

// Forward Declarations:
class Renderer;
class Input;

class Scene
{
    // Member Methods:
public:
	Scene();
	virtual ~Scene();

    virtual bool Initialise(Renderer& renderer) = 0;
	virtual void Process(float deltaTime, Input& input) = 0;
	virtual void Draw(Renderer& renderer) = 0;
	virtual void DebugDraw() = 0;

protected:
    

private:
    Scene(const Scene& scene);
	Scene& operator=(const Scene& scene);

    // Member Data:
public:
protected:
private:

};

#endif // __SCENE_H__
