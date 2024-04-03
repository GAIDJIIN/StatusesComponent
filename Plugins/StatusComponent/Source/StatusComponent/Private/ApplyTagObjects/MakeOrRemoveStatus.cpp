// Statuses Component by GAIDJIIN


#include "ApplyTagObjects/MakeOrRemoveStatus.h"
#include "StatusesComponent.h"

bool UMakeOrRemoveStatus::ApplyTagLogicInternal(const FStatusesInfo CurrentStatuses, const FStatusesInfo ApplyStatuses,
                                                UStatusesComponent* StatusesCompPtr, FStatusesInfoArray& ResultInfo)
{
	/*StatusesCompPtr->RemoveStatuses(ApplyStatuses.Statuses);
	if(bIsMakeNewStatusOnRemove) StatusesCompPtr->AddStatusesWithInfo(ResultInfo);*/
	StatusesCompPtr->AddStatusesWithInfo(ResultInfo);
	return true;
}
