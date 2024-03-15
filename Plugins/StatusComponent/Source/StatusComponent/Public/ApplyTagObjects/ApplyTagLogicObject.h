// Statuses Component by GAIDJIIN

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StatusesInfo.h"
#include "ApplyTagLogicObject.generated.h"

class UStatusesComponent;

UCLASS(Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class STATUSCOMPONENT_API UApplyTagLogicObject : public UObject
{
	GENERATED_BODY()
	public:
		void ApplyTagLogic(const FStatusesInfo CurrentStatuses, const FStatusesInfo ApplyStatuses, TWeakObjectPtr<UStatusesComponent> StatusesCompPtr, FStatusesInfoArray& ResultInfo);
	/*
	 *RESULT
	 *Add Tag Constant/Temporary(Time)
	 *Remove Tag
	 *Effect and no save applied tag
	 */
};
