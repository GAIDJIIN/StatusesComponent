// Statuses Component by GAIDJIIN

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StatusesInfo.generated.h"

class UApplyTagLogicObject;

UENUM(BlueprintType)
enum EStatusState : uint8
{
	Constant,
	Temporary
};

USTRUCT(BlueprintType)
struct FApplyLogicInfo
{ 
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UApplyTagLogicObject> ApplyTagLogicObject = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayTag CreatedStatus;
	
	FApplyLogicInfo() {}
	FApplyLogicInfo(FGameplayTag NewCreatedStatus, TSubclassOf<UApplyTagLogicObject> NewApplyTagLogicObject) :
	CreatedStatus(NewCreatedStatus), ApplyTagLogicObject(NewApplyTagLogicObject) {}
};

USTRUCT(BlueprintType)
struct FStatusApplyInfo
{ 
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TEnumAsByte<EStatusState>,FApplyLogicInfo> StatusApplyInfo;
	
	FStatusApplyInfo() {}
	FStatusApplyInfo(const TMap<TEnumAsByte<EStatusState>,FApplyLogicInfo>& NewStatusApplyInfo) :
	StatusApplyInfo(NewStatusApplyInfo) {}
};

USTRUCT(BlueprintType)
struct FApplyStatusesInfo : public FTableRowBase
{ 
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FStatusApplyInfo> StatusInfo;
    
	FApplyStatusesInfo() {}
};