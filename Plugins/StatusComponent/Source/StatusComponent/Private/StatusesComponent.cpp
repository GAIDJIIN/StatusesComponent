// Statuses Component by GAIDJIIN


#include "StatusesComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"

void UStatusesComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UStatusesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if(bShowDebug) ShowDebug();
}

UStatusesComponent::UStatusesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UStatusesComponent::GetStatusesInfo(FGameplayTagContainer StatusesToGet, TArray<FStatusesInfo>& ReturnStatuses) const
{
    if(StatusesToGet.IsEmpty()) return false;
    auto LocalTags = StatusesToGet.GetGameplayTagArray();
    //ZAGLUSHKA
    return true;
    //////////////////////
}

bool UStatusesComponent::GetStatusInfo(FGameplayTag StatusToGet, FStatusesInfo& ReturnStatusInfo) const
{
    if(!StatusToGet.IsValid()) return false;
    TEnumAsByte<EStatusState> LocalStatusState;
    if(!GetStatusState(StatusToGet, LocalStatusState)) return false;
    switch (LocalStatusState)
    {
        case Constant:
            ReturnStatusInfo = FStatusesInfo(StatusToGet.GetSingleTagContainer());
            break;
        case Temporary:
            {
                if(!TemporaryTags.Contains(StatusToGet)) return false;
                const float LocalRemainingTimeStatus = GetWorld()->GetTimerManager().GetTimerRemaining(TemporaryTags.FindRef(StatusToGet));
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

bool UStatusesComponent::GetStatusState(FGameplayTag StatusToCheck, TEnumAsByte<EStatusState>& StatusState) const
{
    if(!GetIsContainStatus(StatusToCheck)) return false;
    StatusState = Constant;
    if(TemporaryTags.Contains(StatusToCheck)) StatusState = Temporary;
    return true;
}

bool UStatusesComponent::GetIsContainStatus(FGameplayTag StatusToFind, bool ExactCheck, bool InverseCondition) const
{
    const bool Result = ExactCheck ? Statuses.HasTagExact(StatusToFind) : Statuses.HasTag(StatusToFind);
    return InverseCondition ? !Result : Result;
}

bool UStatusesComponent::GetIsContainStatuses(FGameplayTagContainer StatusesToFind, bool CheckAll, bool ExactCheck, bool InverseCondition) const
{
    const bool ResultExactCheck = CheckAll ? Statuses.HasAllExact(StatusesToFind) : Statuses.HasAnyExact(StatusesToFind);
    const bool ResultNotExactCheck = CheckAll ? Statuses.HasAll(StatusesToFind) : Statuses.HasAny(StatusesToFind);
    if(ExactCheck) return InverseCondition ? !ResultExactCheck : ResultExactCheck;
    return InverseCondition ? !ResultNotExactCheck : ResultNotExactCheck;
}

// Main Add Statuses Function
bool UStatusesComponent::AddStatusesWithInfo(FStatusesInfoArray StatusesToAdd)
{
    if(StatusesToAdd.StatusesInfo.IsEmpty()) return false;
    bool bLocalResult = false;
    for(auto StatusContainerToAdd : StatusesToAdd.StatusesInfo)
    {
        if(StatusContainerToAdd.StatusesInfo.Statuses.IsEmpty()) continue;
        bool bLocalFlag = false;
        switch (StatusContainerToAdd.StatusesInfo.StatusesState)
        {
            case Constant:
                bLocalFlag = AddStatuses(StatusContainerToAdd.StatusesInfo.Statuses);
                break;
            case Temporary:
                bLocalFlag = AddTemporaryStatuses(StatusContainerToAdd.StatusesInfo.Statuses,StatusContainerToAdd.StatusesInfo.TemporaryTimeStatus,StatusContainerToAdd.bIsClearTimer);
                break;
            default:
                break;
        }
        if(bLocalFlag && !bLocalResult) bLocalResult = true;
    }
    return bLocalResult;
}

bool UStatusesComponent::AddStatus(FGameplayTag StatusToAdd)
{
    if(GetIsContainStatus(StatusToAdd,true) || !StatusToAdd.IsValid()) return false;
    Statuses.AddTag(StatusToAdd);
    OnAddStatus.Broadcast(StatusToAdd);
    return true;
}

bool UStatusesComponent::AddStatuses(FGameplayTagContainer StatusesToAdd)
{
    if(GetIsContainStatuses(StatusesToAdd,true,true) || !StatusesToAdd.IsValid()) return false;
    Statuses.AppendTags(StatusesToAdd);
    for(const auto Tag : StatusesToAdd) OnAddStatus.Broadcast(Tag);
    OnAddStatuses.Broadcast(StatusesToAdd);
    return true;
}

bool UStatusesComponent::RemoveStatuses(FGameplayTagContainer StatusesToRemove)
{
    if(!GetIsContainStatuses(StatusesToRemove,true,true) || !StatusesToRemove.IsValid()) return false;
    Statuses.RemoveTags(StatusesToRemove);
    for(auto Tag : StatusesToRemove)
    {
        ClearTemporaryStatusTimer(Tag);
        OnRemoveStatus.Broadcast(Tag);
    }
    OnRemoveStatuses.Broadcast(StatusesToRemove);
    return true;
}

// Temporary
bool UStatusesComponent::AddTemporaryStatus(FGameplayTag StatusToAdd, const float TimeToDeleteStatus, const bool bClearTimer)
{
    if(!GetWorld() || !StatusToAdd.IsValid()) return false;
    return MakeTemporaryStatus(StatusToAdd,TimeToDeleteStatus,bClearTimer);
}

bool UStatusesComponent::AddTemporaryStatuses(FGameplayTagContainer StatusesToAdd, const float TimeToDeleteStatuses, const bool bClearTimer)
{
    if(!GetWorld() || !StatusesToAdd.IsValid()) return false;
    bool bLocalResult = false;
    for(auto Tag : StatusesToAdd)
    {
        const bool bLocalFlag = MakeTemporaryStatus(Tag,TimeToDeleteStatuses,bClearTimer);
        if(bLocalFlag && !bLocalResult) bLocalResult = true;
    }
    return bLocalResult;
}

void UStatusesComponent::ClearTemporaryStatusTimer(FGameplayTag StatusToClear)
{
    if(TemporaryTags.Contains(StatusToClear))
    {
        FTimerHandle& LocalTimerHandle = TemporaryTags.FindChecked(StatusToClear);
        GetWorld()->GetTimerManager().ClearTimer(LocalTimerHandle);
        TemporaryTags.Remove(StatusToClear);
    }
}

bool UStatusesComponent::MakeTemporaryStatus(FGameplayTag StatusToAdd, const float TimeToDeleteStatus, const bool bClearTimer)
{
    const bool bLocalAddStatus = AddStatus(StatusToAdd);
    if(!bClearTimer && !bLocalAddStatus) return false;
    ClearTemporaryStatusTimer(StatusToAdd);
    FTimerHandle LocalHandle;
    FTimerDelegate TemporaryTagDelegate;
    TemporaryTagDelegate.BindUFunction(this, "RemoveStatuses",StatusToAdd.GetSingleTagContainer());
    GetWorld()->GetTimerManager().SetTimer(LocalHandle,TemporaryTagDelegate,TimeToDeleteStatus,false);
    TemporaryTags.Add(StatusToAdd,LocalHandle);
    return true;
}

void UStatusesComponent::ApplyStatusLogic()
{
}

// Debug
void UStatusesComponent::ShowDebug()
{
    
    FString DebugStringConstantTags = UKismetStringLibrary::JoinStringArray(UKismetStringLibrary::ParseIntoArray(Statuses.ToStringSimple(), ","),"\n");
    UKismetSystemLibrary::PrintString(
        GetWorld(),
        "Constant Statuses:\n--------------------------------\n" + DebugStringConstantTags,
        true,
        true,
        FLinearColor::Red,
        0.0f
        );

    TArray<FGameplayTag> TemporaryTagsKeys;
    TemporaryTags.GetKeys(TemporaryTagsKeys);
    FString DebugStringTemporaryTags;
    for (auto Tag : TemporaryTagsKeys) DebugStringTemporaryTags += Tag.ToString() + " : " + FString::SanitizeFloat(GetWorld()->GetTimerManager().GetTimerRemaining(TemporaryTags.FindChecked(Tag)))+'\n';
    UKismetSystemLibrary::PrintString(
        GetWorld(),
        "Temporary Statuses\n--------------------------------\n" + DebugStringTemporaryTags,
        true,
        true,
        FLinearColor::Red,
        0.0f
        );
}