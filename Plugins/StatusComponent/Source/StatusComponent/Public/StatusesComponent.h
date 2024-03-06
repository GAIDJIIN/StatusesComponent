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
class STATUSCOMPONENT_API UStatusesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatusesComponent();
    
    // Getter
    UFUNCTION(BlueprintCallable, Category="Statuses Component")
        FGameplayTagContainer& GetStatuses() {return Statuses;}
    UFUNCTION(BlueprintCallable, Category="Statuses Component")
        const bool GetIsContainTag(FGameplayTag TagToFind, bool ExactCheck = true, bool InverseCondition = false) const;
    UFUNCTION(BlueprintCallable, Category="Statuses Component")
        const bool GetIsContainTags(FGameplayTagContainer TagsToFind, bool CheckAll = false,bool ExactCheck = true, bool InverseCondition = false) const;

    // Setter
    UFUNCTION(BlueprintCallable, Category="Statuses Component")
        bool AddStatus(FGameplayTag TagToAdd);
    UFUNCTION(BlueprintCallable, Category="Statuses Component")
        bool AddStatuses(FGameplayTagContainer TagsToAdd);
    UFUNCTION(BlueprintCallable, Category="Statuses Component")
        bool AddTemporaryStatus(FGameplayTag TagToAdd, const float TimeToDeleteTag, const bool bClearTimer = false);
    UFUNCTION(BlueprintCallable, Category="Statuses Component")
        bool AddTemporaryStatuses(FGameplayTagContainer TagsToAdd, const float TimeToDeleteTags, const bool bClearTimer = false);
    UFUNCTION(BlueprintCallable, Category="Statuses Component")
        bool RemoveStatus(FGameplayTag TagToRemove);
    UFUNCTION(BlueprintCallable, Category="Statuses Component")
        bool RemoveStatuses(FGameplayTagContainer TagsToRemove);
    UFUNCTION(BlueprintCallable, Category="Statuses Component")
        void RemoveAllStatuses() { RemoveStatuses(Statuses); }

    // Delegates
    UPROPERTY(BlueprintAssignable, Category="Statuses Component")
        FOnAddStatus OnAddStatus;
    UPROPERTY(BlueprintAssignable, Category="Statuses Component")
        FOnAddStatuses OnAddStatuses;
    UPROPERTY(BlueprintAssignable, Category="Statuses Component")
        FOnRemoveStatus OnRemoveStatus;
    UPROPERTY(BlueprintAssignable, Category="Statuses Component")
        FOnRemoveStatuses OnRemoveStatuses;
    
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // Variables
    FGameplayTagContainer Statuses;
    
    UPROPERTY(EditAnywhere, Category="Statuses Component", meta=(bAllowPrivateAccess))
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
