// Copyright 2019 Jackson. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AnimatedTexture2DSource.generated.h"

struct FAnimatedTexture2DFrame
{
	uint32 Index;
	uint32 Width;
	uint32 Height;
	uint32 OffsetX;	// current frame horizontal offset
	uint32 OffsetY;	// current frame vertical offset
	int16 TransparentIndex;	// 0 - based transparent color index for the current palette(or −1 when transparency is disabled), MVAF
	TArray<uint8> PixelIndices;	// pixel indices for the current frame
	TArray<FColor> Palette;	// the current palette
	int32 Time; // delay time
	
	friend FArchive& operator << (FArchive& Ar, FAnimatedTexture2DFrame& Data)
	{
		Ar << Data.Index;
		Ar << Data.Width;
		Ar << Data.Height;
		Ar << Data.OffsetX;
		Ar << Data.OffsetY;
		Ar << Data.TransparentIndex;
		Ar << Data.PixelIndices;
		Ar << Data.Palette;
		Ar << Data.Time;
		return Ar;
	}

	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}
};


/**
 * 
 */
UCLASS()
class ANIMATEDTEXTURE2DRUNTIME_API UAnimatedTexture2DSource : public UObject
{
	GENERATED_BODY()

public:

	// animated texture frames
	TArray< FAnimatedTexture2DFrame> Frames;

	// current image buffer
	TArray<FColor> FrameBuffer;

	uint32 CurrentFrameIndex;

public:
	//~ Begin UObject Interface.
	virtual void Serialize(FArchive& Ar) override;
	//~ End UObject Interface.

public:
	// draw image
	void DrawFrame(class UTexture2D* In_Resource, uint32 FrameIndex);

	virtual uint32 GetSizeX() { return Frames.Num() == 0 ? 0 : Frames[0].Width; };

	virtual uint32 GetSizeY() { return Frames.Num() == 0 ? 0 : Frames[0].Height; };

};