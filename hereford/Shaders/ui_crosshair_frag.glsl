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

	bool isHorizontalLine = abs(pixelCoords.y) < crosshairThickness &&
							abs(pixelCoords.x) > crosshairGap &&
							abs(pixelCoords.x) < crosshairSize.x + crosshairGap;

	bool isHorizontalOutline = abs(pixelCoords.y) < crosshairThickness + outlineThickness &&
							abs(pixelCoords.x) > crosshairGap &&
							abs(pixelCoords.x) < crosshairSize.x + crosshairGap;

	bool isHorizontalOutline2 = abs(pixelCoords.y) < crosshairThickness + outlineThickness &&
							abs(pixelCoords.x) > crosshairSize.x + crosshairGap &&
							abs(pixelCoords.x) < crosshairSize.x + crosshairGap + outlineThickness;

	bool isVerticalLine = abs(pixelCoords.x) < crosshairThickness &&
							abs(pixelCoords.y) > crosshairGap &&
							abs(pixelCoords.y) < crosshairSize.y + crosshairGap;

	bool isVerticalOutline = abs(pixelCoords.x) < crosshairThickness + outlineThickness &&
							abs(pixelCoords.y) > crosshairGap &&
							abs(pixelCoords.y) < crosshairSize.y + crosshairGap;

	bool isVerticalOutline2 = abs(pixelCoords.x) < crosshairThickness + outlineThickness &&
							abs(pixelCoords.y) > crosshairSize.y + crosshairGap &&
							abs(pixelCoords.y) < crosshairSize.y + crosshairGap + outlineThickness;

	if(isHorizontalLine || isVerticalLine)
	{
		FragColor = crosshairColor;
	}
	else if(isHorizontalOutline || isVerticalOutline || isHorizontalOutline2 || isVerticalOutline2)
	{
		FragColor = outlineColor;
	}
	else
	{
		FragColor = vec4(0.0f);
	}

}