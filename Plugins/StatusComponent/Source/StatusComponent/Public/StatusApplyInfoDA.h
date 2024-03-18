// Statuses Component by GAIDJIIN

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "StatusesInfo.h"
#include "StatusApplyInfoDA.generated.h"


UCLASS()
class STATUSCOMPONENT_API UStatusApplyInfoDA : public UPrimaryDataAsset
{
	GENERATED_BODY()
	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
			TMap<FGameplayTag, FApplyStatusInfoByState> StatusInfo;
};
