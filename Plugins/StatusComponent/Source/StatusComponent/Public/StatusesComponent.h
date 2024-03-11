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

USTRUCT(BlueprintType)
struct FTemporaryStatusesInfo
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
        FGameplayTagContainer TemporaryStatuses;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
        float TemporaryTimeForStatuses = 0.00001f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Statuses Component")
        bool bIsClearTimer = false;
    
    FTemporaryStatusesInfo() {}
    FTemporaryStatusesInfo(FGameplayTagContainer NewTemporaryStatuses, const float NewTemporaryTimeForStatuses, const bool bNewIsClearTimer) :
    TemporaryStatuses(NewTemporaryStatuses), TemporaryTimeForStatuses(FMath::Max(0.00001f, NewTemporaryTimeForStatuses)), bIsClearTimer(bNewIsClearTimer) {}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STATUSCOMPONENT_API UStatusesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatusesComponent();
    
    // Getter
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Getter")
        bool GetStatusState(FGameplayTag TagToCheck, TEnumAsByte<EStatusState>& StatusState) const;
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Getter")
        FGameplayTagContainer& GetStatuses() {return Statuses;}
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Getter")
        bool GetIsContainTag(FGameplayTag TagToFind, bool ExactCheck = true, bool InverseCondition = false) const;
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Getter")
        bool GetIsContainTags(FGameplayTagContainer TagsToFind, bool CheckAll = false,bool ExactCheck = true, bool InverseCondition = false) const;

    // Setter
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Add Logic")
        bool AddStatus(FGameplayTag TagToAdd);
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Add Logic")
        bool AddStatuses(FGameplayTagContainer TagsToAdd);
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Add Logic")
        bool AddTemporaryStatus(FGameplayTag TagToAdd, const float TimeToDeleteTag, const bool bClearTimer = false);
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Add Logic")
        bool AddTemporaryStatusesWithInfo(TArray<FTemporaryStatusesInfo> TagsToAdd);
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Add Logic")
        bool AddTemporaryStatuses(FGameplayTagContainer TagsToAdd, const float TimeToDeleteTags, const bool bClearTimer = false);
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Remove Logic")
        bool RemoveStatus(FGameplayTag TagToRemove);
    UFUNCTION(BlueprintCallable, Category="Statuses Component|Remove Logic")
        bool RemoveStatuses(FGameplayTagContainer TagsToRemove);
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
    
    UPROPERTY(EditAnywhere, Category="Statuses Component", meta=(bAllowPrivateAccess))
        bool bShowDebug = false;
    // Functions
    void ShowDebug();

    // Service
    // Variables
    TMap<FGameplayTag,FTimerHandle> TemporaryTags;
    
    // Functions
    void ClearTemporaryTagTimer(FGameplayTag TagToClear);
    bool MakeTemporaryTag(FGameplayTag TagToAdd, const float TimeToDeleteTag, const bool bClearTimer);
    void ApplyTagLogic();
};
