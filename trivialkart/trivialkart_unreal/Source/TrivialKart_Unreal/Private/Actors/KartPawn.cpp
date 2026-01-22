#include "Actors/KartPawn.h"

#include "EnhancedInputComponent.h"
#include "PaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/SpringArmComponent.h"
#include "Infos/TrivialKartPlayerState.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AKartPawn::AKartPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	CollisionComponent->InitBoxExtent(FVector(100.0f, 32.0f, 50.0f));
	
	Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("SpriteComponent"));
	if (Sprite)
	{
		Sprite->AlwaysLoadOnClient = true;
		Sprite->AlwaysLoadOnServer = true;
		Sprite->bOwnerNoSee = false;
		Sprite->bAffectDynamicIndirectLighting = true;
		Sprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Sprite->SetupAttachment(CollisionComponent);
		static FName CollisionProfileName(TEXT("CharacterMesh"));
		Sprite->SetCollisionProfileName(CollisionProfileName);
		Sprite->SetGenerateOverlapEvents(false);
	}
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SocketOffset = FVector(400.0f, 400.0f, 0.0f);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Camera->bUsePawnControlRotation = false;
	Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	Camera->SetOrthoWidth(400.0f);

}

// Called when the game starts or when spawned
void AKartPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKartPawn::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor->ActorHasTag("Finish"))
	{
		if (TWeakObjectPtr CurrentActor = 
			UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()); 
			CurrentActor.IsValid())
		{
			
			SetActorLocation(CurrentActor->GetActorLocation());
		}
	}
}

// Called to bind functionality to input
void AKartPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
		{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKartPawn::Move);
	}
}

void AKartPawn::Move(const FInputActionValue& Value)
{
	if (GetController() != nullptr)
	{
		if (TWeakObjectPtr CurrentPlayerState = 
			Cast<ATrivialKartPlayerState>(GetPlayerState()); CurrentPlayerState.IsValid())
		{
			if (CurrentPlayerState->GetFuel() <= 0.0f)
				return;
			float MoveAxisVector = Value.Get<float>();
			AddActorLocalOffset(FVector(MoveAxisVector * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0.0f, 0.0f));
		
			CurrentPlayerState->ConsumeFuel(FuelConsumptionRate);
			const float Distance = MoveSpeed/1000.0f;
			CurrentPlayerState->AddDistance(Distance);
		}
	}
}

