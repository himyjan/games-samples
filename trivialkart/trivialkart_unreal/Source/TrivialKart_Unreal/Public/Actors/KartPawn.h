#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KartPawn.generated.h"

struct FInputActionValue;
class UBoxComponent;
class UInputAction;
class UPaperFlipbookComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class TRIVIALKART_UNREAL_API AKartPawn : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> CollisionComponent;
	
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPaperFlipbookComponent> Sprite;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category ="Car|Movement")
	float MoveSpeed;
	
	UPROPERTY(EditAnywhere, Category ="Car|Movement")
	float FuelConsumptionRate;
	
public:
	// Sets default values for this pawn's properties
	AKartPawn();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	virtual void Move(const FInputActionValue& Value);

};
