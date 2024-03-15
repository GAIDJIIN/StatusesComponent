// Statuses Component by GAIDJIIN

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "StatusesInfo.h"
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
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Getter")
        bool GetStatusesInfo(FGameplayTagContainer StatusesToGet, TArray<FStatusesInfo>& ReturnStatuses) const;
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Getter")
        bool GetStatusState(FGameplayTag StatusToCheck, TEnumAsByte<EStatusState>& StatusState) const;
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Getter")
        FGameplayTagContainer& GetStatuses() {return Statuses;}
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Getter")
        bool GetIsContainStatus(FGameplayTag StatusToFind, bool ExactCheck = true, bool InverseCondition = false) const;
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Getter")
        bool GetIsContainStatuses(FGameplayTagContainer StatusesToFind, bool CheckAll = false,bool ExactCheck = true, bool InverseCondition = false) const;

    // Setter
    // Blueprint
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Add Logic")
        bool AddStatusesWithInfo(FStatusesInfoArray StatusesToAdd);
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Remove Logic")
        bool RemoveStatuses(FGameplayTagContainer StatusesToRemove);
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Remove Logic")
        void RemoveAllStatuses() { RemoveStatuses(Statuses); }
    // Delegates
    UPROPERTY(BlueprintAssignable, Category="Statuses Component|Delegates")
        FOnAddStatus OnAddStatus;
    UPROPERTY(BlueprintAssignable, Category="Statuses Component|Delegates")
        FOnAddStatuses OnAddStatuses;
    UPROPERTY(BlueprintAssignable, Category="Statuses Component|Delegates")
        FOnRemoveStatus OnRemoveStatus;
    UPROPERTY(BlueprintAssignable, Category="Statuses Component|Delegates")
        FOnRemoveStatuses OnRemoveStatuses;
    
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // Variables
    FGameplayTagContainer Statuses;
    TMap<FGameplayTag,FTimerHandle> TemporaryTags;

    // Debug
    UPROPERTY(EditAnywhere, Category="Statuses Component", meta=(bAllowPrivateAccess))
        bool bShowDebug = false;
    void ShowDebug();
    
    // Functions
    void ClearTemporaryStatusTimer(FGameplayTag StatusToClear);
    bool MakeTemporaryStatus(FGameplayTag StatusToAdd, const float TimeToDeleteStatus, const bool bClearTimer);
    void ApplyStatusLogic();
    bool AddStatus(FGameplayTag StatusToAdd);
    bool AddStatuses(FGameplayTagContainer StatusesToAdd);
    bool AddTemporaryStatus(FGameplayTag StatusToAdd, const float TimeToDeleteStatus, const bool bClearTimer = false);
    bool AddTemporaryStatuses(FGameplayTagContainer StatusesToAdd, const float TimeToDeleteStatuses, const bool bClearTimer = false);
    
    bool GetStatusInfo(FGameplayTag StatusToGet, FStatusesInfo& ReturnStatusInfo) const;
};