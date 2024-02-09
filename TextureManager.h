#pragma once
class TextureManager
{
public:
	static TextureManager* GetInstance();
	void Finalize();

private:
	static TextureManager* instance;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator = (TextureManager&) = delete;

};

