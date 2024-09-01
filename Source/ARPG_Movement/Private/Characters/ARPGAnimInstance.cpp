

#include "Characters/ARPGAnimInstance.h"
#include "Characters/ARPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UARPGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ARPGCharacter = Cast<AARPGCharacter>(TryGetPawnOwner());
	if (ARPGCharacter)
	{
		ARPGCharacterMovement = ARPGCharacter->GetCharacterMovement();
	}
}

void UARPGAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (ARPGCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(ARPGCharacterMovement->Velocity);
	}
}