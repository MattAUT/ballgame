// COMP710 GP Framework 2022
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

struct SDL_Surface;

class Texture
{
    // Member methods:
public:
    Texture();
    ~Texture();

    bool Initialise(const char* pcFilename);
	void LoadTextTexture(const char* text, const char* fontname, int pointsize);
	void LoadSurfaceIntoTexture(SDL_Surface* pSurface);

    void SetActive();

    int GetWidth() const;
    int GetHeight() const;

protected:

private:
    Texture(const Texture& texture);
    Texture& operator=(const Texture& texture);

    // Member data:
public:

protected:
    unsigned int m_uiTextureId;
    int m_iWidth;
    int m_iHeight;

private:

};

#endif // __TEXTURE_H__
