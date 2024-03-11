// Statuses Component by GAIDJIIN

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ApplyTagLogicObject.generated.h"

class UStatusesComponent;

UCLASS(Abstract)
class STATUSCOMPONENT_API UApplyTagLogicObject : public UObject
{
	GENERATED_BODY()
	public:
		void ApplyTagLogic(FGameplayTag CurrentTag, FGameplayTag ApplyTag, TWeakObjectPtr<UStatusesComponent> StatusesCompPtr);
	/*
	 *RESULT
	 *Add Tag Constant/Temporary(Time)
	 *Remove Tag
	 *Effect and no save applied tag
	 */
};
