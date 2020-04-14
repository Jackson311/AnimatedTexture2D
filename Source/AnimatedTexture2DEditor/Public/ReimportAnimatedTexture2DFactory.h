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

	// 实际重新导入的函数，通常调用UFactory的ImportObject（会根据CanCreateNew（）返回该值判断通过UFactory的哪个FactoryCreate[new, text, binary] 函数进行创建新对象），返回值决定是否导入成功。
	virtual EReimportResult::Type Reimport(UObject* InObject) override;

	virtual int32 GetPriority() const override;

};
