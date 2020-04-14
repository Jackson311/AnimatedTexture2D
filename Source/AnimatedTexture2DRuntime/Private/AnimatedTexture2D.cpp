// Copyright 2019 Jackson. All Rights Reserved.

#include "../Public/AnimatedTexture2D.h"
#if WITH_EDITORONLY_DATA
	#include "EditorFramework/AssetImportData.h"  //Engine
#endif

UAnimatedTexture2D::UAnimatedTexture2D()
{
	IsLoop = true;
	PlayRate = 1.f;
	CanTick = true;
}

void UAnimatedTexture2D::Tick(float DeltaTime)
{
	if (!CanTick) return;
	if (DrawTexture2D && GetAnimatedSource())
	{
		G_DeltaTime += DeltaTime;
		float NextFrameTime = GetAnimatedSource()->Frames[GetAnimatedSource()->CurrentFrameIndex].Time / 1000.f * (1.f / PlayRate);
		if (G_DeltaTime > NextFrameTime)
		{

			G_DeltaTime = 0;
			GetAnimatedSource()->DrawFrame(DrawTexture2D, GetAnimatedSource()->CurrentFrameIndex);
			GetAnimatedSource()->CurrentFrameIndex = (GetAnimatedSource()->CurrentFrameIndex + 1) % GetAnimatedSource()->Frames.Num();
			GetResource()->UpdateResource();

			// stop loop
			if (GetAnimatedSource()->CurrentFrameIndex == GetAnimatedSource()->Frames.Num()-1 && !IsLoop)
			{
				CanTick = false;
			}
		}
	}
	else if(GetAnimatedSource() && !DrawTexture2D)
	{
		UpdateResource();
	}

}

FSlateAtlasData UAnimatedTexture2D::GetSlateAtlasData() const
{
	if (AnimatedTexture2DSource)
	{
		FSlateAtlasData SlateAtlasData(DrawTexture2D, FVector2D(0.f,0.f), FVector2D(1.f, 1.f));
		return SlateAtlasData;
	}
	else
	{
		return FSlateAtlasData(nullptr, FVector2D::ZeroVector, FVector2D::ZeroVector);
	}
}

void UAnimatedTexture2D::UpdateResource()
{
	// Generic Texture2D
	DrawTexture2D = NewObject<UTexture2D>();
	if (DrawTexture2D)
	{
		auto Format = PF_B8G8R8A8;

		DrawTexture2D->PlatformData = new FTexturePlatformData();
		DrawTexture2D->PlatformData->SizeX = GetAnimatedSource()->GetSizeX();
		DrawTexture2D->PlatformData->SizeY = GetAnimatedSource()->GetSizeY();
		DrawTexture2D->PlatformData->PixelFormat = Format;

		// Allocate first mipmap.
		int32 NumBlocksX = GetAnimatedSource()->GetSizeX();
		int32 NumBlocksY = GetAnimatedSource()->GetSizeY();
		FTexture2DMipMap* Mip = new FTexture2DMipMap();
		DrawTexture2D->PlatformData->Mips.Add(Mip);
		Mip->SizeX = NumBlocksX;
		Mip->SizeY = NumBlocksY;
		Mip->BulkData.Lock(LOCK_READ_WRITE);
		Mip->BulkData.Realloc(NumBlocksX * NumBlocksY * sizeof(FColor));
		Mip->BulkData.Unlock();

		DrawTexture2D->CompressionSettings = TextureCompressionSettings::TC_Default;
		DrawTexture2D->SRGB = true;
	}

}

#if WITH_EDITOR
void UAnimatedTexture2D::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (IsLoop)
		CanTick = true;
	else
	{
		if (DrawTexture2D && GetAnimatedSource())
		{
			GetAnimatedSource()->DrawFrame(DrawTexture2D, 0);
		}
	}
}

#endif

void UAnimatedTexture2D::PostInitProperties()
{
#if WITH_EDITORONLY_DATA
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_NeedLoad))
	{
		AssetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
	}
#endif
	Super::PostInitProperties();
}

void UAnimatedTexture2D::PostLoad()
{
	Super::PostLoad();

#if WITH_EDITORONLY_DATA
	if (AssetImportData == nullptr)
	{
		AssetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
	}

	if (!SourceFilePath_DEPRECATED.IsEmpty())
	{
		FAssetImportInfo Info;
		Info.Insert(FAssetImportInfo::FSourceFile(SourceFilePath_DEPRECATED));
		AssetImportData->SourceData = MoveTemp(Info);
	}
#endif
}

#if WITH_EDITORONLY_DATA
void UAnimatedTexture2D::GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const
{
	if (AssetImportData)
	{
		OutTags.Add(FAssetRegistryTag(SourceFileTagName(), AssetImportData->GetSourceData().ToJson(), FAssetRegistryTag::TT_Hidden));
	}

	Super::GetAssetRegistryTags(OutTags);
}
#endif

void UAnimatedTexture2D::SetAnimatedTexture2DSource(UAnimatedTexture2DSource * In_Source)
{
	AnimatedTexture2DSource = In_Source;
}

UAnimatedTexture2DSource * UAnimatedTexture2D::GetAnimatedSource()
{
	return AnimatedTexture2DSource;
}




