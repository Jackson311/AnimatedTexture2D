// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Copyright 2019 Jackson. All Rights Reserved.

#include "AnimatedTexture2DEditor.h"
#include "ThumbnailRendering/ThumbnailManager.h"
#include "AnimatedTexture2D.h"
#include "AnimatedTextureThumbnailRenderer.h"

#define LOCTEXT_NAMESPACE "FAnimatedTexture2DEditorModule"

void FAnimatedTexture2DEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UThumbnailManager::Get().RegisterCustomRenderer(UAnimatedTexture2D::StaticClass(), UAnimatedTextureThumbnailRenderer::StaticClass());

}

void FAnimatedTexture2DEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
	if (UObjectInitialized() && GIsEditor)
	{
		UThumbnailManager::Get().UnregisterCustomRenderer(UAnimatedTexture2D::StaticClass());
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAnimatedTexture2DEditorModule, AnimatedTexture2DEditor)