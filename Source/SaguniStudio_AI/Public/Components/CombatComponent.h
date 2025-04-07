// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamaged, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SAGUNISTUDIO_AI_API UCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCombatComponent();

protected:
    virtual void BeginPlay() override;


public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION()
    void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void ApplyDamage(AActor* DamagedActor, float DamageAmount, AController* Instigator, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass);

    // Function to heal
    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(float HealAmount);

    // Get current health
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealth() const { return Health; }

    // Check if dead
    UFUNCTION(BlueprintPure, Category = "Health")
    bool IsDead() const { return Health <= 0.0f; }

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnHealthChanged OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDamaged OnDamaged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDeath OnDeath;

private:
    UPROPERTY(EditAnywhere, Category = "Health")
    float MaxHealth = 100.0f;

    float Health;
};