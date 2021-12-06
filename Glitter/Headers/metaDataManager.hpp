#ifndef META_DATA_MANAGER
#define META_DATA_MANAGER
#pragma once

extern int mWidth;
extern int mHeight;


extern float deltaTime;
extern float currentTime;
extern float lastFrame;

static class MetaDataManager
{



public:
	static MetaDataManager& Instance()
	{
		static MetaDataManager instance;
		return instance;
	}
	int mWidth = 1280;
	int mHeight = 800;


	float deltaTime = 0;
	float currentTime = 0;
	float lastFrame = 0;


	
	//~MetaDataManager();
	/*MetaDataManager(MetaDataManager const&) = delete;
	void operator=(MetaDataManager const&) = delete;*/

private:
	MetaDataManager();
	MetaDataManager(MetaDataManager const&);              // Don't Implement
	MetaDataManager& operator=(MetaDataManager const&); // Don't implement
	//static MetaDataManager* instance;
};
#endif

