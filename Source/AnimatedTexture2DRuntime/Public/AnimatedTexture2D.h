// Copyright 2019 Jackson. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "AnimatedTexture2DSource.h"
#include "Tickable.h"
#include "Slate/SlateTextureAtlasInterface.h"
#include "AnimatedTexture2D.generated.h"


/**
 * 
 */
UCLASS()
class ANIMATEDTEXTURE2DRUNTIME_API UAnimatedTexture2D : public UObject, public FTickableGameObject,public ISlateTextureAtlasInterface
{
	GENERATED_BODY()

public:
	UAnimatedTexture2D();

	UPROPERTY(EditAnywhere, Category = "Animated Info")
		bool IsLoop;

	UPROPERTY(EditAnywhere, Category = "Animated Info", meta = (ClampMin = 0.001f, ClampMax = 10.f))
		float PlayRate;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
		FString SourceFilePath_DEPRECATED;

	UPROPERTY()
		class UAssetImportData* AssetImportData;
#endif // WITH_EDITORONLY_DATA
public:
	
	//~ Begin FTickableGameObject Interface.
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const
	{
		return Super::GetStatID();
	}
	virtual bool IsTickableInEditor() const
	{
		return true;
	}
	virtual bool IsTickable() const override
	{
		return true;
	}
	virtual UWorld* GetTickableGameObjectWorld() const
	{
		return GetWorld();
	}
	//~ End FTickableGameObject Interface.

	//~ Begin ISlateTextureAtlasInterface Interface.
	virtual FSlateAtlasData GetSlateAtlasData() const;
	//~ End ISlateTextureAtlasInterface Interface.

	// create DrawTexture2D
	virtual void UpdateResource();

	// get DrawTexture2D
	UTexture2D* GetResource() { return DrawTexture2D; };

	//~ Begin UObject Interface.
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void PostInitProperties() override;
	virtual void PostLoad() override;
	virtual void GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const override;

	//~ Begin UObject Interface.

	void SetAnimatedTexture2DSource(class UAnimatedTexture2DSource* In_Source);

	UAnimatedTexture2DSource* GetAnimatedSource();

private:

	// AnimatedTexture2DSource
	UPROPERTY()
		UAnimatedTexture2DSource* AnimatedTexture2DSource;

	// calculate delay time
	float G_DeltaTime;

	// tick animation
	bool CanTick;

	// temp resource
	UPROPERTY(Transient)
		UTexture2D* DrawTexture2D;
};
