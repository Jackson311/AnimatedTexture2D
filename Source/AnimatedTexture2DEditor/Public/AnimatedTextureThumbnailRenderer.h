// Copyright 2019 Jackson. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ThumbnailRendering/TextureThumbnailRenderer.h"
#include "AnimatedTextureThumbnailRenderer.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATEDTEXTURE2DEDITOR_API UAnimatedTextureThumbnailRenderer : public UTextureThumbnailRenderer
{
	GENERATED_BODY()
public:

	//~ Begin UTextureThumbnailRenderer Interface.
	virtual void GetThumbnailSize(UObject* Object, float Zoom, uint32& OutWidth, uint32& OutHeight) const override;
	virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* Viewport, FCanvas* Canvas) override;
	//~ End UTextureThumbnailRenderer Interface.

	void UpdateDrawTexture(const int32 SrcWidth, const int32 SrcHeight, const TArray<FColor> &ColorData);
	
	UPROPERTY(Transient)
	class UTexture2D* DrawTexture2D;
};
