// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Copyright 2019 Jackson. All Rights Reserved.

#include "AnimatedTexture2DRuntime.h"

#define LOCTEXT_NAMESPACE "FAnimatedTexture2DRuntimeModule"

void FAnimatedTexture2DRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FAnimatedTexture2DRuntimeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAnimatedTexture2DRuntimeModule, AnimatedTexture2DRuntime)