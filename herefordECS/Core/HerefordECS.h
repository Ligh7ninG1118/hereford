#pragma once

class HerefordECS
{
public:
	HerefordECS(unsigned int inWidth = 1920, unsigned int inHeight = 1080);
	~HerefordECS();

	void OnInit();
	void OnUpdate();
	void OnDestroy();

	void OnKeyDown(unsigned char inKey);
	void OnKeyUp(unsigned char inKey);

	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }
	void SetWidth(int inWidth) { mWidth = inWidth; };
	void SetHeight(int inHeight) { mHeight = inHeight; }

private:
	int mWidth;
	int mHeight;

};

