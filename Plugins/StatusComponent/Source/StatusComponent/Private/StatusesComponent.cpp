// Statuses Component by GAIDJIIN


#include "StatusesComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"

void UStatusesComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bShowDebug) ShowDebug();
}

UStatusesComponent::UStatusesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Getter Info

bool UStatusesComponent::GetStatusesInfo(const FGameplayTagContainer& StatusesToGet,
                                         TArray<FStatusesInfo>& ReturnStatuses) const
{
	if (StatusesToGet.IsEmpty()) return false;
	auto LocalStatuses = StatusesToGet.GetGameplayTagArray();
	bool bLocalResult = false;
	for (auto LocalStatus : LocalStatuses)
	{
		FStatusesInfo LocalReturnStatusInfo;
		const bool bLocalSuccess = GetStatusInfo(LocalStatus, LocalReturnStatusInfo);
		if (bLocalSuccess)
		{
			ReturnStatuses.Add(LocalReturnStatusInfo);
			if (!bLocalResult) bLocalResult = true;
		}
	}
	return bLocalResult;
}

bool UStatusesComponent::GetStatusInfo(const FGameplayTag& StatusToGet, FStatusesInfo& ReturnStatusInfo) const
{
	if (!StatusToGet.IsValid() || !GetWorld()) return false;
	TEnumAsByte<EStatusState> LocalStatusState;
	if (!GetStatusState(StatusToGet, LocalStatusState)) return false;
	switch (LocalStatusState)
	{
	case Constant:
		{
			if (!GetIsContainStatus(StatusToGet)) return false;
			ReturnStatusInfo = FStatusesInfo(StatusToGet.GetSingleTagContainer());
			break;
		}
	case Temporary:
		{
			if (!TemporaryTags.Contains(StatusToGet)) return false;
			const float LocalRemainingTimeStatus = GetWorld()->GetTimerManager().GetTimerRemaining(
				TemporaryTags.FindRef(StatusToGet));
			ReturnStatusInfo = FStatusesInfo(
				StatusToGet.GetSingleTagContainer(),
				Temporary,
				LocalRemainingTimeStatus
			);
			break;
		}
	default:
		break;
	}
	return true;
}

bool UStatusesComponent::GetStatusState(const FGameplayTag& StatusToCheck, TEnumAsByte<EStatusState>& StatusState) const
{
	if (!GetIsContainStatus(StatusToCheck)) return false;
	StatusState = Constant;
	if (TemporaryTags.Contains(StatusToCheck)) StatusState = Temporary;
	return true;
}

bool UStatusesComponent::GetStatusesInfoByState(const TEnumAsByte<EStatusState> StatusState,
	TArray<FStatusesInfo>& ReturnStatusesInfo) const
{
	if(Statuses.IsEmpty()) return false;
	TArray<FStatusesInfo> LocalReturnStatuses;
	if(!GetStatusesInfo(Statuses,LocalReturnStatuses)) return false;
	ReturnStatusesInfo.Empty();
	ReturnStatusesInfo = LocalReturnStatuses.FilterByPredicate([&](const FStatusesInfo LocalStatus)
	{ return LocalStatus.StatusesState == StatusState; });
	return !ReturnStatusesInfo.IsEmpty();
}

bool UStatusesComponent::GetStatusesByState(const TEnumAsByte<EStatusState> StatusState,
	FGameplayTagContainer& ReturnStatuses) const
{
	if(Statuses.IsEmpty()) return false;
	TArray<FStatusesInfo> LocalReturnStatuses;
	if(!GetStatusesInfo(Statuses,LocalReturnStatuses)) return false;
	for(const auto LocalStatus : LocalReturnStatuses)
	{
		if(LocalStatus.StatusesState != StatusState) continue;
		ReturnStatuses.AppendTags(LocalStatus.Statuses);
	}
	return !ReturnStatuses.IsEmpty();
}

bool UStatusesComponent::GetAllStatusesInfo(TArray<FStatusesInfo>& ReturnStatusesInfo) const
{
	const auto LocalStatusesInfo = GetStatusesInfo(Statuses, ReturnStatusesInfo);
	return LocalStatusesInfo;
}

bool UStatusesComponent::GetIsContainStatus(const FGameplayTag& StatusToFind, const bool ExactCheck,
                                            const bool InverseCondition) const
{
	const bool Result = ExactCheck ? Statuses.HasTagExact(StatusToFind) : Statuses.HasTag(StatusToFind);
	return InverseCondition ? !Result : Result;
}

bool UStatusesComponent::GetIsContainStatuses(const FGameplayTagContainer& StatusesToFind, const bool CheckAll,
                                              const bool ExactCheck, const bool InverseCondition) const
{
	const bool ResultExactCheck =
		CheckAll ? Statuses.HasAllExact(StatusesToFind) : Statuses.HasAnyExact(StatusesToFind);
	const bool ResultNotExactCheck = CheckAll ? Statuses.HasAll(StatusesToFind) : Statuses.HasAny(StatusesToFind);
	if (ExactCheck) return InverseCondition ? !ResultExactCheck : ResultExactCheck;
	return InverseCondition ? !ResultNotExactCheck : ResultNotExactCheck;
}

// Main Add Statuses Logic

bool UStatusesComponent::AddStatusesWithInfo(const FStatusesInfoArray& StatusesToAdd)
{
	if (StatusesToAdd.StatusesInfo.IsEmpty()) return false;
	bool bLocalResult = false;
	
	// Filter Get Can added tag container for call delegate added tags
	TArray<FGameplayTag> LocalOldStatusesArray;
	Statuses.GetGameplayTagArray(LocalOldStatusesArray);
	
	for (auto StatusContainerToAdd : StatusesToAdd.StatusesInfo)
	{
		if (StatusContainerToAdd.StatusesInfo.Statuses.IsEmpty()) continue;
		bool bLocalFlag = false;
		switch (StatusContainerToAdd.StatusesInfo.StatusesState)
		{
		case Constant:
			bLocalFlag = AddStatuses(StatusContainerToAdd.StatusesInfo.Statuses);
			break;
		case Temporary:
			bLocalFlag = AddTemporaryStatuses(StatusContainerToAdd.StatusesInfo.Statuses, StatusContainerToAdd.StatusesInfo.TemporaryTimeStatus,
			                     StatusContainerToAdd.bIsClearTimer);
			break;
		default:
			break;
		}
		if(bLocalFlag) bLocalResult = true;
	}

	// Check New Gameplay Tag
	TArray<FGameplayTag> LocalNewStatusesArray;
	Statuses.GetGameplayTagArray(LocalNewStatusesArray);
	const auto LocalAddedStatuses = FGameplayTagContainer::CreateFromArray(
		LocalNewStatusesArray.FilterByPredicate([&](const FGameplayTag LocalFindedTag)
		{
			return !LocalOldStatusesArray.Contains(LocalFindedTag);
		})
		);
	
	if (bLocalResult && !LocalAddedStatuses.IsEmpty()) OnAddStatuses.Broadcast(LocalAddedStatuses);
	return bLocalResult;
}

bool UStatusesComponent::AddStatus(const FStatusesInfo& StatusToAdd)
{
	if (GetIsContainStatuses(StatusToAdd.Statuses, true) || !StatusToAdd.Statuses.IsValid()) return false;
	Statuses.AppendTags(StatusToAdd.Statuses);
	return true;
}

bool UStatusesComponent::AddStatuses(const FGameplayTagContainer& StatusesToAdd)
{
	if (GetIsContainStatuses(StatusesToAdd, true, true) || !StatusesToAdd.IsValid()) return false;
	for (auto Tag : StatusesToAdd)
	{
		FStatusesInfo LocalAddTag(Tag.GetSingleTagContainer());
		AddStatus(LocalAddTag);
	}
	return true;
}

bool UStatusesComponent::RemoveStatuses(const FGameplayTagContainer StatusesToRemove)
{
	if (!GetIsContainStatuses(StatusesToRemove, false, true) || !StatusesToRemove.IsValid()) return false;
	FGameplayTagContainer RemovedTag;
	for (auto Tag : StatusesToRemove)
	{
		if (!GetIsContainStatus(Tag)) continue;
		RemovedTag.AddTag(Tag);
		ClearTemporaryStatusTimer(Tag);
	}
	if (RemovedTag.IsEmpty()) return false;
	Statuses.RemoveTags(RemovedTag);
	OnRemoveStatuses.Broadcast(RemovedTag);
	return true;
}

// Temporary Logic

bool UStatusesComponent::AddTemporaryStatuses(const FGameplayTagContainer& StatusesToAdd,
                                              const float TimeToDeleteStatuses, const bool bClearTimer)
{
	if (!GetWorld() || !StatusesToAdd.IsValid()) return false;
	bool bLocalResult = false;
	for (auto Tag : StatusesToAdd)
	{
		const bool bLocalFlag = MakeTemporaryStatus(Tag, TimeToDeleteStatuses, bClearTimer);
		if (bLocalFlag) bLocalResult = true;
	}
	return bLocalResult;
}

bool UStatusesComponent::ClearTemporaryStatusTimer(const FGameplayTag& StatusToClear)
{
	if (!TemporaryTags.Contains(StatusToClear) || !GetWorld()) return false;
	FTimerHandle& LocalTimerHandle = TemporaryTags.FindChecked(StatusToClear);
	GetWorld()->GetTimerManager().ClearTimer(LocalTimerHandle);
	TemporaryTags.Remove(StatusToClear);
	return true;
}

bool UStatusesComponent::MakeTemporaryStatus(const FGameplayTag& StatusToAdd, const float TimeToDeleteStatus,
                                             const bool bClearTimer)
{
	if(!GetWorld()) return false;
	const FStatusesInfo LocalAddTempTag(StatusToAdd.GetSingleTagContainer(),EStatusState::Temporary,TimeToDeleteStatus);
	const bool bLocalAddStatus = AddStatus(LocalAddTempTag);
	if (!bClearTimer && !bLocalAddStatus) return false;
	ClearTemporaryStatusTimer(StatusToAdd);
	FTimerHandle LocalHandle;
	FTimerDelegate TemporaryTagDelegate;
	TemporaryTagDelegate.BindUFunction(this, "RemoveStatuses", StatusToAdd.GetSingleTagContainer());
	GetWorld()->GetTimerManager().SetTimer(LocalHandle, TemporaryTagDelegate, TimeToDeleteStatus, false);
	TemporaryTags.Add(StatusToAdd, LocalHandle);
	return true;
}

// Debug Logic

void UStatusesComponent::ShowDebug()
{
	if(!GetWorld()) return;

	// Show Constant Statuses
	FGameplayTagContainer LocalConstantStatuses;
	GetStatusesByState(Constant,LocalConstantStatuses);
	FString DebugStringConstantTags = UKismetStringLibrary::JoinStringArray(
		UKismetStringLibrary::ParseIntoArray(LocalConstantStatuses.ToStringSimple(), ","), "\n");
	UKismetSystemLibrary::PrintString(
		GetWorld(),
		"Constant Statuses:\n--------------------------------\n" + DebugStringConstantTags,
		true,
		true,
		FLinearColor::Red,
		0.0f
	);

	// Show Temporary Statuses
	TArray<FGameplayTag> TemporaryTagsKeys;
	TemporaryTags.GetKeys(TemporaryTagsKeys);
	FString DebugStringTemporaryTags;
	for (auto Tag : TemporaryTagsKeys) DebugStringTemporaryTags += Tag.ToString() + " : " + FString::SanitizeFloat(
		GetWorld()->GetTimerManager().GetTimerRemaining(TemporaryTags.FindChecked(Tag))) + '\n';
	UKismetSystemLibrary::PrintString(
		GetWorld(),
		"Temporary Statuses\n--------------------------------\n" + DebugStringTemporaryTags,
		true,
		true,
		FLinearColor::Red,
		0.0f
	);
}

const FText UStatusesComponent::GetStatusesReadableText(const FGameplayTagContainer StatusesToText) const
{
	return FText::FromString(UKismetStringLibrary::JoinStringArray(
		UKismetStringLibrary::ParseIntoArray(StatusesToText.ToStringSimple(), ","), "\n"));
}