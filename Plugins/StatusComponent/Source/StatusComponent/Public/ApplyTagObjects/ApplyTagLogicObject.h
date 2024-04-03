// Statuses Component by GAIDJIIN

#pragma once

#include "CoreMinimal.h"
#include "StatusesInfo.h"
#include "ApplyTagLogicObject.generated.h"

class UStatusesComponent;

UCLASS(Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class STATUSCOMPONENT_API UApplyTagLogicObject : public UObject
{
	GENERATED_BODY()
	public:
	// Functions
		bool ApplyTag(const FStatusesInfo CurrentStatuses, const FStatusesInfo ApplyStatuses, TWeakObjectPtr<UStatusesComponent> StatusesCompPtr, FStatusesInfoArray& ResultInfo);

	protected:
	// Function that can be derived in BP class
	UFUNCTION(BlueprintNativeEvent)
		bool ApplyTagLogic(const FStatusesInfo CurrentStatuses, const FStatusesInfo ApplyStatuses, UStatusesComponent* StatusesCompPtr, FStatusesInfoArray& ResultInfo);
		virtual bool ApplyTagLogicInternal(const FStatusesInfo CurrentStatuses, const FStatusesInfo ApplyStatuses, UStatusesComponent* StatusesCompPtr, FStatusesInfoArray& ResultInfo);
	
	// Properties

	public:
	/*
	 *Use for logic in Statuses Logic (add/remove statuses, make temp and other operations)
	 *Implement them your way
	*/
	UPROPERTY(EditAnywhere, Category="Object Result Info")
		TArray<FStatusesInfo> StatusesResultLogicObject;

	/*
	 *RESULT
	 *Add Tag Constant/Temporary(Time)
	 *Remove Tag
	 *Effect and no save applied tag
	 */
};
