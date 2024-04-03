// Statuses Component by GAIDJIIN


#include "ApplyTagObjects/ApplyTagLogicObject.h"

bool UApplyTagLogicObject::ApplyTag(const FStatusesInfo CurrentStatuses, const FStatusesInfo ApplyStatuses,
                                    TWeakObjectPtr<UStatusesComponent> StatusesCompPtr, FStatusesInfoArray& ResultInfo)
{
	if(!StatusesCompPtr.IsValid()) return false;
	return ApplyTagLogic(CurrentStatuses, ApplyStatuses, StatusesCompPtr.Get(), ResultInfo);
}

bool UApplyTagLogicObject::ApplyTagLogic_Implementation(const FStatusesInfo CurrentStatuses,
	const FStatusesInfo ApplyStatuses, UStatusesComponent* StatusesCompPtr, FStatusesInfoArray& ResultInfo)
{
	return ApplyTagLogicInternal(CurrentStatuses, ApplyStatuses, StatusesCompPtr, ResultInfo);
}

bool UApplyTagLogicObject::ApplyTagLogicInternal(const FStatusesInfo CurrentStatuses, const FStatusesInfo ApplyStatuses,
                                                 UStatusesComponent* StatusesCompPtr, FStatusesInfoArray& ResultInfo)
{
	return false;
}
