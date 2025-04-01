#pragma once

#include "Math/Math.h"

class GameWorld;

class HerefordECS
{
public:
	HerefordECS(uint16 inWidth = 1920, uint16 inHeight = 1080);
	~HerefordECS();

	void OnInit();
	void OnUpdate();
	void OnDestroy();

	void OnKeyDown(unsigned char inKey);
	void OnKeyUp(unsigned char inKey);

	uint16 GetWidth() const { return mWidth; }
	uint16 GetHeight() const { return mHeight; }
	void SetWidth(uint16 inWidth) { mWidth = inWidth; };
	void SetHeight(uint16 inHeight) { mHeight = inHeight; }

private:
	uint16 mWidth;
	uint16 mHeight;



};

