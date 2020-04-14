// Copyright 2019 Jackson. All Rights Reserved.

#include "AnimatedTextureThumbnailRenderer.h"
#include "AnimatedTexture2D.h"
#include "AnimatedTexturePH.h"
#include "Engine/Texture2D.h"

void UAnimatedTextureThumbnailRenderer::GetThumbnailSize(UObject* Object, float Zoom, uint32& OutWidth, uint32& OutHeight) const
{
	UAnimatedTexture2D* AnimatedTexture2D = Cast<UAnimatedTexture2D>(Object);
	if (AnimatedTexture2D && AnimatedTexture2D->GetResource())
	{
		OutHeight = AnimatedTexture2D->GetResource()->GetSizeY();
		OutWidth = AnimatedTexture2D->GetResource()->GetSizeX();
		return;
	}
	OutHeight = OutWidth = 0;
	return;
	
}


void UAnimatedTextureThumbnailRenderer::Draw(UObject * Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget * Viewport, FCanvas * Canvas)
{
	UAnimatedTexture2D* AnimatedTexture2D = Cast<UAnimatedTexture2D>(Object);
	if (!AnimatedTexture2D) return;
	if (!AnimatedTexture2D->GetResource()) return;
	//UpdateDrawTexture(AnimatedTexture2D->GetAnimatedSource()->Global_Width, AnimatedTexture2D->GetAnimatedSource()->Global_Height, AnimatedTexture2D->GetAnimatedSource()->FrameBuffer);
	return Super::Draw(AnimatedTexture2D->GetResource(), X, Y, Width, Height, Viewport, Canvas);

}

void UAnimatedTextureThumbnailRenderer::UpdateDrawTexture(const int32 SrcWidth, const int32 SrcHeight, const TArray<FColor> &ColorData)
{
	if (ColorData.Num() == 0)
	{
		UE_LOG(LogAnimatedTexture2D, Error, TEXT("ERROR:0002"));
		return;
	}


	// 创建Texture2D纹理
	if(!DrawTexture2D)
		DrawTexture2D = UTexture2D::CreateTransient(SrcWidth, SrcHeight, PF_B8G8R8A8);

	// 锁住他的数据，以便修改
	uint8* MipData = static_cast<uint8*>(DrawTexture2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

	// 创建纹理数据
	uint8* DestPtr = NULL;
	const FColor* SrcPtr = NULL;
	for (int32 y = 0; y < SrcHeight; y++)
	{
		int DestPtrlength = (SrcHeight - 1 - y) * SrcWidth * sizeof(FColor);
		DestPtr = &MipData[DestPtrlength];

		int num = (SrcHeight - 1 - y) * SrcWidth;
		SrcPtr = const_cast<FColor*>(&ColorData[num]);
		for (int32 x = 0; x < SrcWidth; x++)
		{
			*DestPtr++ = SrcPtr->B;
			*DestPtr++ = SrcPtr->G;
			*DestPtr++ = SrcPtr->R;
			*DestPtr++ = 0xFF;
			SrcPtr++;
		}
	}

	// 解锁纹理
	DrawTexture2D->PlatformData->Mips[0].BulkData.Unlock();
	DrawTexture2D->UpdateResource();

	return;
}

