#include "group.h"
#define mNumFacePtrs (this->numFacePtrs)
#define mFacePtrs (this->facePtrs)
#define mIDs (this->IDs)
#define mFaceAmts (this->faceAmts)



int group_load(Group *this,Objbin *preObject,int groupIndex)
{
	memset(this,0,sizeof(Group));

	mNumFacePtrs = preObject->ptrsByGroup[groupIndex];
	mFacePtrs = malloc(mNumFacePtrs*sizeof(uint32_t));
	if (mFacePtrs == NULL)
		goto error;

	mIDs = malloc(mNumFacePtrs*sizeof(uint32_t));
	if (mIDs == NULL)
		goto error;

	mFaceAmts = malloc(mNumFacePtrs*sizeof(uint32_t));
	if (mFaceAmts == NULL)
		goto error;

	uint32_t startingFacePtr = 0;
	for (int i=0;i<groupIndex;i++)
		startingFacePtr += preObject->ptrsByGroup[i];

	memcpy(mFacePtrs,preObject->facePtrs+startingFacePtr,mNumFacePtrs*sizeof(uint32_t));
	memcpy(mIDs,preObject->IDs+startingFacePtr,mNumFacePtrs*sizeof(uint32_t));
	memcpy(mFaceAmts,preObject->faceAmts+startingFacePtr,mNumFacePtrs*sizeof(uint32_t));

	return 1;
error:
	group_free(this);
	return 0;
}
void group_free(Group *this)
{
	free(mFacePtrs);
	free(mIDs);
	free(mFaceAmts);
}

#undef mFacePtrs
#undef mIDs
#undef mFaceAmts
#undef mNumFacePtrs
