// Copyright 2019 Jackson. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "AnimatedTexture2DFactory.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATEDTEXTURE2DEDITOR_API UAnimatedTexture2DFactory : public UFactory
{
	GENERATED_UCLASS_BODY()
	
public:
	//UAnimatedTexture2DFactory(const FObjectInitializer& ObjectInitializer);
	
	//~ Begin UFactory Interface
	virtual bool FactoryCanImport(const FString & Filename);
	virtual UObject* FactoryCreateBinary(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn) override;
	//~ End UFactory Interface

	// ------------------------------
	// read gif frame
	// ------------------------------
	// [In] Data buffer
	// [In] Whdr struct frame 
	static void ReadAnimatedTextureFrame(void* Data, struct GIF_WHDR* Whdr);
};
