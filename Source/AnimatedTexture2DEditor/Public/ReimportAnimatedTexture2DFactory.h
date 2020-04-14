// Copyright 2019 Jackson. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimatedTexture2DFactory.h"
#include "EditorReimportHandler.h"

#include "ReimportAnimatedTexture2DFactory.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATEDTEXTURE2DEDITOR_API UReimportAnimatedTexture2DFactory : public UAnimatedTexture2DFactory, public  FReimportHandler
{
	GENERATED_UCLASS_BODY()

public:

	virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;

	virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;

	// ʵ�����µ���ĺ�����ͨ������UFactory��ImportObject�������CanCreateNew�������ظ�ֵ�ж�ͨ��UFactory���ĸ�FactoryCreate[new, text, binary] �������д����¶��󣩣�����ֵ�����Ƿ���ɹ���
	virtual EReimportResult::Type Reimport(UObject* InObject) override;

	virtual int32 GetPriority() const override;

};
