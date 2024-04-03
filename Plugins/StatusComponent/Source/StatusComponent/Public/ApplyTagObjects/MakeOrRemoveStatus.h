// Statuses Component by GAIDJIIN

#pragma once

#include "CoreMinimal.h"
#include "ApplyTagLogicObject.h"
#include "MakeOrRemoveStatus.generated.h"

UCLASS()
class STATUSCOMPONENT_API UMakeOrRemoveStatus : public UApplyTagLogicObject
{
	GENERATED_BODY()
	public:
		virtual bool ApplyTagLogicInternal(const FStatusesInfo CurrentStatuses, const FStatusesInfo ApplyStatuses, UStatusesComponent* StatusesCompPtr, FStatusesInfoArray& ResultInfo) override;

	private:
	UPROPERTY(EditAnywhere, Category="Object Logic Info")
		bool bIsMakeNewStatusOnRemove = true;
		
};
