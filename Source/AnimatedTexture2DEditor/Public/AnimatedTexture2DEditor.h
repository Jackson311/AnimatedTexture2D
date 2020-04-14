// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Copyright 2019 Jackson. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FAnimatedTexture2DEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
