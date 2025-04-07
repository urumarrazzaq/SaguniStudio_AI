// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "ITypes.h"
#include "AIControllerBase.generated.h"

class AEnemyBase;

UCLASS()
class SAGUNISTUDIO_AI_API AAIControllerBase : public AAIController
{
    GENERATED_BODY()

public:
    AAIControllerBase();

    virtual void OnPossess(APawn* InPawn) override;

    virtual void OnUnPossess() override;

protected:

    virtual void BeginPlay() override;

public:

    UPROPERTY()
    AEnemyBase* Enemy;

    UPROPERTY(BlueprintReadOnly)
    AActor* TargetActor;

    UFUNCTION(BlueprintCallable, Category = "Targeting")
    void SetTargetActor(AActor* NewTarget) { TargetActor = NewTarget; };

    UFUNCTION(BlueprintCallable, Category = "Targeting")
    AActor* GetTargetActor() const { return TargetActor; };

    UFUNCTION(BlueprintCallable, Category = "Perception")
    void HandleSenseSight(AActor* InActor);

    UFUNCTION(BlueprintCallable, Category = "Perception")
    void HandleSenseHearing(AActor* InActor, FVector InLocation);

    UFUNCTION(BlueprintCallable, Category = "Perception")
    void HandleSenseDamage(AActor* InActor);

    UFUNCTION(BlueprintCallable, Category = "AI Utilities")
    EAIPositionRelation GetRelativePositionToPlayer(AActor* Player);

    UFUNCTION(BlueprintCallable, Category = "AI Utilities")
    FVector GetTargetPositionAroundPlayer(AActor* Player, float DistanceOffset);

    UFUNCTION(BlueprintCallable, Category = "AI Utilities")
    FVector RetreatFromPlayer(AActor* Player, float RetreatDistance);

    UFUNCTION(BlueprintCallable, Category = "AI Utilities")
    FVector MoveBehindPlayer(AActor* Player = nullptr, float Distance = 500.f, float SideOffset = 300.f);

    // Blackboard Keys
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard Key")
    FName AttackTargetKeyName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard Key")
    FName AttackRadiusKeyName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard Key")
    FName DefendRadiusKeyName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard Key")
    FName AIStateKeyName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard Key")
    FName AIAttackingStateKeyName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard Key")
    FName PointOfInterestKeyName;

    // AI State management
    UFUNCTION(BlueprintCallable, Category = "AI State")
    EAIStates GetCurrentState() const;

    UFUNCTION(BlueprintCallable, Category = "AI State")
    EAttackingStates GetCurrentAttackingState() const;

    UFUNCTION(BlueprintCallable, Category = "AI State")
    void SetAIState(EAIStates AIState);

    UFUNCTION(BlueprintCallable, Category = "AI State")
    void SetAIAttackingState(EAttackingStates  AIAttackingState);

    UFUNCTION(BlueprintCallable, Category = "AI State")
    void SetStateAsIdle();

    UFUNCTION(BlueprintCallable, Category = "AI State")
    void SetStateAsPatrolling();

    UFUNCTION(BlueprintCallable, Category = "AI State")
    void SetStateAsAttacking(AActor* AttackTarget);

    UFUNCTION(BlueprintCallable, Category = "AI State")
    void SetStateAsInvestigating(FVector Location);

    UFUNCTION(BlueprintCallable, Category = "AI State")
    void SetStateAsDead();

private:

    UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    UBlackboardComponent* BlackboardComp;

    void RunAI();

    void StopAI();

    void SetupPerceptionSystem();

    UPROPERTY(VisibleAnywhere, Category = "AI")
    UAIPerceptionComponent* AIPerceptionComponent;

    UPROPERTY(VisibleAnywhere, Category = "AI")
    UAISenseConfig_Sight* SightConfig;

    UPROPERTY(VisibleAnywhere, Category = "AI")
    UAISenseConfig_Hearing* HearingConfig;

    UPROPERTY(VisibleAnywhere, Category = "AI")
    UAISenseConfig_Damage* DamageConfig;

    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

    UFUNCTION()
    void InitializeBlackBoardKeys();
};
