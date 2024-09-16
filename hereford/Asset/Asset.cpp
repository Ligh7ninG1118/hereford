#include "Asset.h"

Asset::Asset(const std::string& inPath, EAssetType inType)
	:
	mPath(inPath),
	mType(inType)
{
}

Asset::~Asset()
{
	printf("Asset Destructor\n");
}
