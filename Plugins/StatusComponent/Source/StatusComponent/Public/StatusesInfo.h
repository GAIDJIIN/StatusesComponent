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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component",
		meta=(ClampMin="0.00001", EditCondition = "StatusesState==EStatusState::Temporary", EditConditionHides))
	float TemporaryTimeStatus = 0.00001f;

	FStatusesInfo()
	{
	}

	FStatusesInfo(const FGameplayTagContainer NewStatuses, TEnumAsByte<EStatusState> NewStatusesState = Constant,
	              const float NewTemporaryTimeStatus = 0.00001) :
		Statuses(NewStatuses), StatusesState(NewStatusesState),
		TemporaryTimeStatus(FMath::Max(0.00001f, NewTemporaryTimeStatus))
	{
	}
};

USTRUCT(BlueprintType)
struct FStatusesWithAddInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
		FStatusesInfo StatusesInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
		bool bIsClearTimer = false;

	FStatusesWithAddInfo()
	{
	}

	FStatusesWithAddInfo(const FStatusesInfo NewStatuses, const bool bNewIsClearTimer) :
		StatusesInfo(NewStatuses), bIsClearTimer(bNewIsClearTimer)
	{
	}
};

USTRUCT(BlueprintType)
struct FStatusesInfoArray
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
		TArray<FStatusesWithAddInfo> StatusesInfo;

	FStatusesInfoArray()
	{
	}

	FStatusesInfoArray(const TArray<FStatusesWithAddInfo> NewStatuses) : StatusesInfo(NewStatuses)
	{
	}
};