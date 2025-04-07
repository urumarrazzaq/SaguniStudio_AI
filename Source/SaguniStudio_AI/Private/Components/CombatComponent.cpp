// CombatComponent.cpp
#include "Components/CombatComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "GameFramework/DamageType.h"

UCombatComponent::UCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();
    Health = MaxHealth;

    // Register the OnTakeDamage event
    AActor* Owner = GetOwner();
    if (Owner)
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &UCombatComponent::OnTakeDamage);
    }
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UCombatComponent::Heal(float HealAmount)
{
    if (Health <= 0.0f) return;

    Health = FMath::Clamp(Health + HealAmount, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}

void UCombatComponent::ApplyDamage(AActor* DamagedActor, float DamageAmount, AController* Instigator, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass)
{
    if (!DamagedActor) return;
        
    UGameplayStatics::ApplyDamage(DamagedActor, DamageAmount, Instigator, DamageCauser, DamageTypeClass);
}

void UCombatComponent::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    UE_LOG(LogTemp, Warning, TEXT("Received Damage: %f | Damage Causer: %s | Instigated By: %s"),
        Damage,
        *GetNameSafe(DamageCauser),
        *GetNameSafe(InstigatedBy));

    if (Health <= 0.0f) return;

    Health -= Damage;
    OnHealthChanged.Broadcast(Health);
    OnDamaged.Broadcast(DamageCauser);
    UE_LOG(LogTemp, Warning, TEXT("%s took %f damage!"), *DamagedActor->GetName(), Damage);

    if (Health <= 0.0f)
    {
        OnDeath.Broadcast();
        UE_LOG(LogTemp, Warning, TEXT("%s has died!"), *DamagedActor->GetName());
        //DamagedActor->Destroy();
    }
}

