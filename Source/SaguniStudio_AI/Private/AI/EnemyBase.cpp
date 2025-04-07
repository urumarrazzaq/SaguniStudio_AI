


#include "AI/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AITypes.h"


AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	Sword->SetupAttachment(GetMesh(), TEXT("sword"));
	Sword->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));


	Tags.Add(FName("Enemy"));
}



void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}



void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::SetAIMovementSpeed(EAIMovementMode MovementMode)
{
    float NewSpeed = 0.f;

    switch (MovementMode)
    {
    case EAIMovementMode::Idle:
        NewSpeed = 0.f;
        break;
    case EAIMovementMode::Walk:
        NewSpeed = 150.f;
        break;
    case EAIMovementMode::Jog:
        NewSpeed = 400.f;
        break;
    case EAIMovementMode::Sprint:
        NewSpeed = 600.f;
        break;
    case EAIMovementMode::Faster:
        NewSpeed = 750.f;
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown movement mode!"));
        return;
    }

    GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
    UE_LOG(LogTemp, Warning, TEXT("AI Movement Speed set to: %f"), NewSpeed);
}

void AEnemyBase::Attack_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("C++ Default Attack!"));
}

