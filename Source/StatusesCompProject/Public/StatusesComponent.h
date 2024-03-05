// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "StatusesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddStatus,FGameplayTag, AddStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddStatuses,FGameplayTagContainer, AddStatuses);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoveStatus,FGameplayTag,RemoveStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoveStatuses,FGameplayTagContainer,RemoveStatuses);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STATUSESCOMPPROJECT_API UStatusesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatusesComponent();
    
    // Getter
    UFUNCTION(BlueprintCallable)
        FGameplayTagContainer& GetStatuses() {return Statuses;}
    UFUNCTION(BlueprintCallable)
        const bool GetIsContainTag(FGameplayTag TagToFind, bool ExactCheck = true, bool InverseCondition = false) const;
    UFUNCTION(BlueprintCallable)
        const bool GetIsContainTags(FGameplayTagContainer TagsToFind, bool CheckAll = false,bool ExactCheck = true, bool InverseCondition = false) const;

    // Setter
    UFUNCTION(BlueprintCallable)
        bool AddStatus(FGameplayTag TagToAdd);
    UFUNCTION(BlueprintCallable)
        bool AddStatuses(FGameplayTagContainer TagsToAdd);
    UFUNCTION(BlueprintCallable)
        bool AddTemporaryStatus(FGameplayTag TagToAdd, const float TimeToDeleteTag, const bool bClearTimer = false);
    UFUNCTION(BlueprintCallable)
        bool AddTemporaryStatuses(FGameplayTagContainer TagsToAdd, const float TimeToDeleteTags, const bool bClearTimer = false);
    UFUNCTION(BlueprintCallable)
        void RemoveStatus(FGameplayTag TagToRemove);
    UFUNCTION(BlueprintCallable)
        void RemoveStatuses(FGameplayTagContainer TagsToRemove);

    // Delegates
    UPROPERTY(BlueprintAssignable)
        FOnAddStatus OnAddStatus;
    UPROPERTY(BlueprintAssignable)
        FOnAddStatuses OnAddStatuses;
    UPROPERTY(BlueprintAssignable)
        FOnRemoveStatus OnRemoveStatus;
    UPROPERTY(BlueprintAssignable)
        FOnRemoveStatuses OnRemoveStatuses;
    
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // Variables
    FGameplayTagContainer Statuses;
    
    UPROPERTY(EditAnywhere, meta=(bAllowPrivateAccess))
        bool bShowDebug = false;
    // Functions
    void ShowDebug();

    // Service
    // Variables
    TMap<FGameplayTag,FTimerHandle> TemporaryTags;
    
    // Functions
    void ClearTemporaryTagTimer(FGameplayTag& TagToClear);
    bool MakeTemporaryTag(FGameplayTag TagToAdd, const float TimeToDeleteTag, const bool bClearTimer);
};
