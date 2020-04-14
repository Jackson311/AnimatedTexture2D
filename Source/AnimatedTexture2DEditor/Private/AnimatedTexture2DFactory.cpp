// Copyright 2019 Jackson. All Rights Reserved.

#include "AnimatedTexture2DFactory.h"
#include "AnimatedTexture2D.h"
#include "AnimatedTexturePH.h"
#include "EditorFramework/AssetImportData.h"
#include "Editor.h"

#include "Engine/Texture2D.h" //Engine
#include "RenderUtils.h" //RenderCore

#define GIF_MGET(m,s,a,c) if (c) m = (uint8*)FMemory::Malloc(s); else FMemory::Free(m);
#include "gif_load/gif_load.h" // from: https://github.com/hidefromkgb/gif_load


UAnimatedTexture2DFactory::UAnimatedTexture2DFactory(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	Formats.Add(TEXT("gif;Texture"));
	SupportedClass = UAnimatedTexture2D::StaticClass();
	bCreateNew = false;
	bEditorImport = true;

	ImportPriority = DefaultImportPriority + 1;

}

bool UAnimatedTexture2DFactory::FactoryCanImport(const FString & Filename)
{
	FString Extension = FPaths::GetExtension(Filename, true);
	return true;
	//return Extension.Compare(TEXT(".gif"), ESearchCase::IgnoreCase);
}

UObject * UAnimatedTexture2DFactory::FactoryCreateBinary(UClass * Class, UObject * InParent, FName Name, EObjectFlags Flags, UObject * Context, const TCHAR * Type, const uint8 *& Buffer, const uint8 * BufferEnd, FFeedbackContext * Warn)
{
	//FEditorDelegates::OnAssetPreImport.Broadcast(this, Class, InParent, Name, Type); // 4.21 4.22
	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPreImport(this, Class, InParent, Name, Type); // 4.23 4.24

	// create a animated source ---- Serialize need InParent 
	UAnimatedTexture2DSource* AnimatedSource = NewObject<UAnimatedTexture2DSource>(InParent);
	
	// texture already exists
	UAnimatedTexture2D* ExistingAnimatedTexture2D = FindObject<UAnimatedTexture2D>(InParent, *Name.ToString());


	// read gif frame
	int Result = GIF_Load((void*)Buffer, BufferEnd - Buffer, UAnimatedTexture2DFactory::ReadAnimatedTextureFrame, 0, (void*)AnimatedSource, 0L);
	if (Result == 0)
		return nullptr;

	UAnimatedTexture2D* AnimatedTexture2D;

	// Replace Aniamted texture
	if (ExistingAnimatedTexture2D)
	{
		AnimatedTexture2D = ExistingAnimatedTexture2D;
		AnimatedTexture2D->IsLoop = ExistingAnimatedTexture2D->IsLoop;
		AnimatedTexture2D->PlayRate = ExistingAnimatedTexture2D->PlayRate;
		AnimatedTexture2D->SetAnimatedTexture2DSource(AnimatedSource);
	}
	else
	{
		AnimatedTexture2D = Cast<UAnimatedTexture2D>(CreateOrOverwriteAsset(Class, InParent, Name, Flags));
		AnimatedTexture2D->SetAnimatedTexture2DSource(AnimatedSource);
	}
	// create draw texute2d
	AnimatedTexture2D->UpdateResource();

	// update AssetImportData
	AnimatedTexture2D->AssetImportData->Update(CurrentFilename, FileHash.IsValid() ? &FileHash : nullptr);


	//FEditorDelegates::OnAssetPostImport.Broadcast(this, AnimatedTexture2D); // 4.21 4.22
	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPostImport(this, AnimatedTexture2D); // 4.23 4.24

	return AnimatedTexture2D;
}

void UAnimatedTexture2DFactory::ReadAnimatedTextureFrame(void* Data, struct GIF_WHDR* Whdr)
{
	UAnimatedTexture2DSource* AnimatedSource = (UAnimatedTexture2DSource*)Data;
	if (!AnimatedSource)
	{
		UE_LOG(LogAnimatedTexture2D, Log, TEXT("ERROR CODE:0001"));
		return;
	}

	// first frame
	if (Whdr->ifrm == 0)
	{
	}

	// ******************
	// create new frame
	//

	// current new frame
	FAnimatedTexture2DFrame CurrentFrame;
	
	CurrentFrame.Index = Whdr->ifrm;
	CurrentFrame.Height = Whdr->fryd;
	CurrentFrame.Width = Whdr->frxd;
	CurrentFrame.OffsetX = Whdr->frxo;
	CurrentFrame.OffsetY = Whdr->fryo;
	CurrentFrame.TransparentIndex = Whdr->tran;
	
	if (Whdr->time >= 0)
		CurrentFrame.Time = Whdr->time * 10;
	else
		CurrentFrame.Time = 33;

	// ******************
	// init CurrentFrame PixelIndices
	//

	uint32 PixelIndicesNum = CurrentFrame.Height*CurrentFrame.Width;
	CurrentFrame.PixelIndices.Init(0, PixelIndicesNum);
	memcpy(CurrentFrame.PixelIndices.GetData(), Whdr->bptr, PixelIndicesNum);


	// ******************
	// init CurrentFrame Palette
	//

	uint32 PaletteNum = Whdr->clrs;
	CurrentFrame.Palette.Init(FColor(), PaletteNum);
	for (uint32 i = 0; i < PaletteNum; ++i)
	{
		FColor PaletteColor(0, 0, 0, 255);
		PaletteColor.B = Whdr->cpal[i].B;
		PaletteColor.G = Whdr->cpal[i].G;
		PaletteColor.R = Whdr->cpal[i].R;
		CurrentFrame.Palette[i] = PaletteColor;
	}


	// ******************
	// Add Frame
	//
	AnimatedSource->Frames.Add(CurrentFrame);

}
