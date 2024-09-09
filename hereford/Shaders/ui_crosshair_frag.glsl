#version 460 core

in vec2 TexCoords;

uniform vec2 screenDimension;
uniform vec2 crosshairSize;
uniform float crosshairGap;
uniform float crosshairHeat;
uniform float crosshairThickness;
uniform float outlineThickness;
uniform vec4 crosshairColor;
uniform vec4 outlineColor;

out vec4 FragColor;

void main()
{
	vec2 pixelCoords = TexCoords * screenDimension;
	float absX = abs(pixelCoords.x),
		  absY = abs(pixelCoords.y);

	bool isHorizontalLine = absY < crosshairThickness &&
							absX > crosshairGap &&
							absX < crosshairSize.x + crosshairGap;

	bool isHorizontalOutline = absY < crosshairThickness + outlineThickness &&
							absX > crosshairGap &&
							absX < crosshairSize.x + crosshairGap + outlineThickness;

	bool isVerticalLine = absX < crosshairThickness &&
							absY > crosshairGap &&
							absY < crosshairSize.y + crosshairGap;

	bool isVerticalOutline = absX < crosshairThickness + outlineThickness &&
							absY > crosshairGap &&
							absY < crosshairSize.y + crosshairGap + outlineThickness;

	if(isHorizontalLine || isVerticalLine)
	{
		FragColor = crosshairColor;
	}
	else if(isHorizontalOutline || isVerticalOutline)
	{
		FragColor = outlineColor;
	}
	else
	{
		FragColor = vec4(0.0f);
	}

}