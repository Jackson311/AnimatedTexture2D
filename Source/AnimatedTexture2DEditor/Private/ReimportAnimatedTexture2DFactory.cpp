// Copyright 2019 Jackson. All Rights Reserved.

#include "ReimportAnimatedTexture2DFactory.h"
#include "Engine/FontFace.h"
#include "AnimatedTexture2D.h"
#include "EditorFramework/AssetImportData.h"


UReimportAnimatedTexture2DFactory::UReimportAnimatedTexture2DFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	SupportedClass = UAnimatedTexture2D::StaticClass();

	bCreateNew = false;
}

bool UReimportAnimatedTexture2DFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	UAnimatedTexture2D* AnimatedTexture2D = Cast<UAnimatedTexture2D>(Obj);
	if (AnimatedTexture2D) {
		AnimatedTexture2D->AssetImportData->ExtractFilenames(OutFilenames);
		return true;
	}
	return false;

}

void UReimportAnimatedTexture2DFactory::SetReimportPaths(UObject * Obj, const TArray<FString>& NewReimportPaths)
{
	UAnimatedTexture2D* AnimatedTexture2D = Cast<UAnimatedTexture2D>(Obj);
	if (AnimatedTexture2D && ensure(NewReimportPaths.Num() == 1))
	{
		AnimatedTexture2D->AssetImportData->UpdateFilenameOnly(NewReimportPaths[0]);
	}

}

EReimportResult::Type UReimportAnimatedTexture2DFactory::Reimport(UObject * InObject)
{
	UAnimatedTexture2D* AnimatedTexture2D = Cast<UAnimatedTexture2D>(InObject);

	if(!AnimatedTexture2D)
		return EReimportResult::Failed;

	const FString ResolvedSourceFilePath = AnimatedTexture2D->AssetImportData->GetFirstFilename();
	bool bOutCanceled = false;
	UObject * NewObject = ImportObject(InObject->GetClass(), InObject->GetOuter(), InObject->GetFName(), InObject->GetFlags(), ResolvedSourceFilePath, nullptr, bOutCanceled);
	if (NewObject)
	{
		return EReimportResult::Succeeded;
	}

	return EReimportResult::Failed;
}


int32 UReimportAnimatedTexture2DFactory::GetPriority() const
{
	return ImportPriority;
}