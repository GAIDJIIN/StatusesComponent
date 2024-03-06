// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusesComponent.h"
#include "Kismet/KismetSystemLibrary.h"


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

const bool UStatusesComponent::GetIsContainTag(FGameplayTag TagToFind, bool ExactCheck, bool InverseCondition) const
{
    bool Result = ExactCheck ? Statuses.HasTagExact(TagToFind) : Statuses.HasTag(TagToFind);
    return InverseCondition ? !Result : Result;
}

const bool UStatusesComponent::GetIsContainTags(FGameplayTagContainer TagsToFind, bool CheckAll, bool ExactCheck, bool InverseCondition) const
{
    bool ResultExactCheck = CheckAll ? Statuses.HasAllExact(TagsToFind) : Statuses.HasAnyExact(TagsToFind);
    bool ResultNotExactCheck = CheckAll ? Statuses.HasAll(TagsToFind) : Statuses.HasAny(TagsToFind);
    if(ExactCheck) return InverseCondition ? !ResultExactCheck : ResultExactCheck;
    return InverseCondition ? !ResultNotExactCheck : ResultNotExactCheck;
}

bool UStatusesComponent::AddStatus(FGameplayTag TagToAdd)
{
    if(GetIsContainTag(TagToAdd,true) || !TagToAdd.IsValid()) return false;
    Statuses.AddTag(TagToAdd);
    OnAddStatus.Broadcast(TagToAdd);
    return true;
}

bool UStatusesComponent::AddStatuses(FGameplayTagContainer TagsToAdd)
{
    if(GetIsContainTags(TagsToAdd,true,true) || !TagsToAdd.IsValid()) return false;
    Statuses.AppendTags(TagsToAdd);
    for(auto Tag : TagsToAdd) OnAddStatus.Broadcast(Tag);
    OnAddStatuses.Broadcast(TagsToAdd);
    return true;
}

bool UStatusesComponent::RemoveStatus(FGameplayTag TagToRemove)
{
    if(!Statuses.RemoveTag(TagToRemove) || !TagToRemove.IsValid()) return false;
    ClearTemporaryTagTimer(TagToRemove);
    OnRemoveStatus.Broadcast(TagToRemove);
    return true;
}

bool UStatusesComponent::RemoveStatuses(FGameplayTagContainer TagsToRemove)
{
    if(!GetIsContainTags(TagsToRemove,true,true) || !TagsToRemove.IsValid()) return false;
    Statuses.RemoveTags(TagsToRemove);
    for(auto Tag : TagsToRemove)
    {
        ClearTemporaryTagTimer(Tag);
        OnRemoveStatus.Broadcast(Tag);
    }
    OnRemoveStatuses.Broadcast(TagsToRemove);
    return true;
}

// Temporary
bool UStatusesComponent::AddTemporaryStatus(FGameplayTag TagToAdd, const float TimeToDeleteTag, const bool bClearTimer)
{
    if(!GetWorld() || !TagToAdd.IsValid()) return false;
    return MakeTemporaryTag(TagToAdd,TimeToDeleteTag,bClearTimer);
}

bool UStatusesComponent::AddTemporaryStatuses(FGameplayTagContainer TagsToAdd, const float TimeToDeleteTags, const bool bClearTimer)
{
    if(!GetWorld() || !TagsToAdd.IsValid()) return false;
    bool bLocalResult = false;
    bool bLocalFlag;
    for(auto Tag : TagsToAdd)
    {
        bLocalFlag = MakeTemporaryTag(Tag,TimeToDeleteTags,bClearTimer);
        if(bLocalFlag && !bLocalResult) bLocalResult = true;
    }
    return bLocalResult;
}

void UStatusesComponent::ClearTemporaryTagTimer(FGameplayTag& TagToClear)
{
    auto bIsContainTemporaryTag = TemporaryTags.Contains(TagToClear);
    if(bIsContainTemporaryTag)
    {
        FTimerHandle& LocalTimerHandle = TemporaryTags.FindChecked(TagToClear);
        GetWorld()->GetTimerManager().ClearTimer(LocalTimerHandle);
        TemporaryTags.Remove(TagToClear);
    }
}

bool UStatusesComponent::MakeTemporaryTag(FGameplayTag TagToAdd, const float TimeToDeleteTag, const bool bClearTimer)
{
    const bool bLocalAddStatus = AddStatus(TagToAdd);
    if(!bClearTimer && !bLocalAddStatus) return false;
    ClearTemporaryTagTimer(TagToAdd);
    FTimerHandle LocalHandle;
    FTimerDelegate TemporaryTagDelegate;
    TemporaryTagDelegate.BindUFunction(this, "RemoveStatus",TagToAdd);
    GetWorld()->GetTimerManager().SetTimer(LocalHandle,TemporaryTagDelegate,TimeToDeleteTag,false);
    TemporaryTags.Add(TagToAdd,LocalHandle);
    return true;
}

// Debug
void UStatusesComponent::ShowDebug()
{
    UKismetSystemLibrary::PrintString(
        GetWorld(),
        "Statuses" + Statuses.ToString(),
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