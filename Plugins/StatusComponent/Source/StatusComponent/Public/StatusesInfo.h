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
struct FStatusesInfo
{ 
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
		FGameplayTagContainer Statuses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
		TEnumAsByte<EStatusState> StatusesState = Constant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component", meta=(ClampMin="0.00001", EditCondition = "StatusesState==EStatusState::Temporary", EditConditionHides))
		float TemporaryTimeStatus = 0.00001f;
	
	FStatusesInfo() {}
	FStatusesInfo(const FGameplayTagContainer NewStatuses, TEnumAsByte<EStatusState> NewStatusesState = Constant, const float NewTemporaryTimeStatus = 0.00001) :
	Statuses(NewStatuses), StatusesState(NewStatusesState), TemporaryTimeStatus(FMath::Max(0.00001f,NewTemporaryTimeStatus)) {}
};

USTRUCT(BlueprintType)
struct FStatusesWithAddInfo
{ 
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
		FStatusesInfo StatusesInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
		bool bIsClearTimer = false;
	
	FStatusesWithAddInfo() {}
	FStatusesWithAddInfo(const FStatusesInfo NewStatuses, const bool bNewIsClearTimer) :
	StatusesInfo(NewStatuses), bIsClearTimer(bNewIsClearTimer) {}
};

USTRUCT(BlueprintType)
struct FStatusesInfoArray
{ 
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
		TArray<FStatusesWithAddInfo> StatusesInfo;
	
	FStatusesInfoArray() {}
	FStatusesInfoArray(const TArray<FStatusesWithAddInfo> NewStatuses) : StatusesInfo(NewStatuses) {}
};

USTRUCT(BlueprintType, meta=(DisplayName="ResultLogicInfo"))
struct FApplyLogicInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category="Statuses Component")
		TObjectPtr<UApplyTagLogicObject> ApplyTagLogicObject = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
		TArray<FStatusesInfo> ResultInfo;
	
	FApplyLogicInfo() {}
	FApplyLogicInfo(const TObjectPtr<UApplyTagLogicObject> NewApplyTagLogicObject, const TArray<FStatusesInfo> NewResultInfo) :
	ApplyTagLogicObject(NewApplyTagLogicObject), ResultInfo(NewResultInfo) {}
};

USTRUCT(BlueprintType)
struct FApplyLogicInfoArray
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
		TArray<FApplyLogicInfo> ApplyLogicInfoArray;

	FApplyLogicInfoArray() {}
	FApplyLogicInfoArray(const TArray<FApplyLogicInfo> NewApplyLogicInfoArray) :
	ApplyLogicInfoArray(NewApplyLogicInfoArray) {}
};

USTRUCT(BlueprintType)
struct FApplicableStatusesInfo
{ 
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
		TMap<TEnumAsByte<EStatusState>,FApplyLogicInfoArray> ApplicableStatusesInfo;
	
	FApplicableStatusesInfo() {}
	FApplicableStatusesInfo(const TMap<TEnumAsByte<EStatusState>,FApplyLogicInfoArray> NewApplicableStatusesInfo) :
	ApplicableStatusesInfo(NewApplicableStatusesInfo) {}
};

USTRUCT(BlueprintType)
struct FApplicableStatusInfoByState
{ 
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
		FGameplayTagContainer ApplicableStatuses;
	// Check All statuses to Apply
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
		bool bCheckAllStatuses = false;
	// For select a more preferred tag application based on priority (the higher the priority, the more preferable)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
		int32 ApplyPriority = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
		FApplicableStatusesInfo StatusApplyInfo;
	
	FApplicableStatusInfoByState() {}
	FApplicableStatusInfoByState(const FGameplayTagContainer NewApplicableStatuses, const FApplicableStatusesInfo NewStatusApplyInfo) :
	ApplicableStatuses(NewApplicableStatuses), StatusApplyInfo(NewStatusApplyInfo) {}
};

USTRUCT(BlueprintType)
struct FApplicableStatusInfoArray
{ 
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
	TArray<FApplicableStatusInfoByState> ApplicableStatusInfoArray;
	
	FApplicableStatusInfoArray() {}
	FApplicableStatusInfoArray(const TArray<FApplicableStatusInfoByState> NewApplicableStatusesInfo) :
	ApplicableStatusInfoArray(NewApplicableStatusesInfo) {}
};

USTRUCT(BlueprintType)
struct FApplyStatusInfoByState
{ 
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
		TMap<TEnumAsByte<EStatusState>,FApplicableStatusInfoArray> StatusApplyInfo;
	
	FApplyStatusInfoByState() {}
	FApplyStatusInfoByState(const TMap<TEnumAsByte<EStatusState>,FApplicableStatusInfoArray>& NewStatusApplyInfo) :
	StatusApplyInfo(NewStatusApplyInfo) {}
};

// Struct for setup apply statuses by DataTable
USTRUCT(BlueprintType, meta=(DisplayName="ResultLogicInfo"))
struct FApplyStatusInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
		TMap<FGameplayTag, FApplyStatusInfoByState> StatusInfo;
    
	FApplyStatusInfo() {}
};