// Copyright 2019 Jackson. All Rights Reserved.

#include "AnimatedTexture2DSource.h"
#include "AnimatedTexture2D.h"
#include "Engine/Texture2D.h"
#include "RenderingThread.h"

void UAnimatedTexture2DSource::Serialize(FArchive & Ar)
{
	Super::Serialize(Ar);
	Ar << Frames;
}

void UAnimatedTexture2DSource::DrawFrame(UTexture2D * In_Resource, uint32 FrameIndex)
{
	if (FrameBuffer.Num() == 0)
	{
		FrameBuffer.Init(FColor(), GetSizeX()*GetSizeY());
	}

	//-- Render Command Data
	struct FRenderCommandData
	{
		UTexture2D * Resource;
		UAnimatedTexture2DSource* Source;
		uint32 FrameIndex;
	};

	typedef TSharedPtr<FRenderCommandData, ESPMode::ThreadSafe> FCommandDataPtr;
	FCommandDataPtr CommandData = MakeShared<FRenderCommandData, ESPMode::ThreadSafe>();

	CommandData->Resource = In_Resource;
	CommandData->Source = this;
	CommandData->FrameIndex = FrameIndex;

	//-- Equeue command
	ENQUEUE_RENDER_COMMAND(DecodeGIFFrameToTexture)(
		[CommandData](FRHICommandListImmediate& RHICmdList)
		{
			if (!CommandData->Resource)
				return;

			UTexture2D* DrawTexture2D = CommandData->Resource;
			UAnimatedTexture2DSource* AnimatedTexture2DSource = CommandData->Source;
			uint32 CurrentFrameCount = CommandData->FrameIndex;
			FAnimatedTexture2DFrame& Frame = AnimatedTexture2DSource->Frames[CurrentFrameCount];
			
			if (!DrawTexture2D->PlatformData)
				return;
			if (DrawTexture2D->PlatformData->Mips.Num() == 0)
				return;
			if (DrawTexture2D->PlatformData->Mips[0].BulkData.IsLocked())
				return;

			uint32 TexWidth = DrawTexture2D->GetSizeX();
			uint32 TexHeight = DrawTexture2D->GetSizeY();

			uint32 FrameWidth = Frame.Width;
			uint32 FrameHeight = Frame.Height;

			FColor* PICT = AnimatedTexture2DSource->FrameBuffer.GetData();

			// 资源消耗最多地方
			for (uint32 y = 0; y < FrameHeight; ++y)
			{
				uint32 PixelIndex = y * FrameWidth;
				uint32 PICTIndexTemp = (y + Frame.OffsetY) * TexWidth;
				for (uint32 x = 0; x < FrameWidth; ++x)
				{
					uint8& PaletteIndex = Frame.PixelIndices[PixelIndex];
					FColor& PixelColor = Frame.Palette[PaletteIndex];
					++PixelIndex;

					if (PaletteIndex == Frame.TransparentIndex)
						continue;

					uint32 PICTIndex = PICTIndexTemp + (x + Frame.OffsetX);
					PICT[PICTIndex] = PixelColor;
				}
			}

			

			// lock data
			FColor* MipData = static_cast<FColor*>(DrawTexture2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
			int32 MipNum = TexWidth * TexHeight * sizeof(FColor);
			memcpy(MipData, PICT, MipNum);
			// unlock data
			DrawTexture2D->PlatformData->Mips[0].BulkData.Unlock();

			return;
			
		}
	);
}
