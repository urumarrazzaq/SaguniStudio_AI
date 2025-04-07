// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/StaticMeshComponent.h"
#include <Components/CombatComponent.h>
#include "ITypes.h"
#include "EnemyBase.generated.h"


UCLASS()
class SAGUNISTUDIO_AI_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

	/*UPROPERTY(BlueprintReadWrite)
	class AAIControllerBase* PawnController;*/


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	float CurrentHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	float HealPotionNumber = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	int AttackCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	bool IsSaveAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	bool IsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	int Combo = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	bool IsMontagePlaying = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties")
	bool IsDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* Sword;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	EAIStates AIState = EAIStates::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EAttackingStates AIAttackingState = EAttackingStates::Simple;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetAIMovementSpeed(EAIMovementMode MovementMode);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void PossessedBy(AController* NewController) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void Attack();
	virtual void Attack_Implementation();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;
};
