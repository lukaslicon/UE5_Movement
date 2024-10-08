



#include "Characters/ARPGCharacter.h"

#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"


AARPGCharacter::AARPGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
	ViewCamera->bUsePawnControlRotation = true;


}


void AARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("Character"));

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem <UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterContext, 0);
		}
	}
}

void AARPGCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
		const FVector2D MovementVector = Value.Get<FVector2D>();

		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementVector.Y);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
}

void AARPGCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (GetController()) {
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
	}

}

void AARPGCharacter::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon) 
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		OverlappingWeapon->SetOwner(this);
		OverlappingWeapon->SetInstigator(this);
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon; 
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
}


void AARPGCharacter::EquipKeyPressed()
{
	if (CanUnEquip())
	{
		UE_LOG(LogTemp, Warning, TEXT("UnEquipping weapon"));
		PlayEquipMontage(FName("Unequip"));
		CharacterState = ECharacterState::ECS_Unequipped;
		ActionState = EActionState::EAS_EquippingWeapon;
	}
	else if (CanEquip())
	{
		UE_LOG(LogTemp, Warning, TEXT("Equipping weapon"));
		PlayEquipMontage(FName("Equip"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		ActionState = EActionState::EAS_EquippingWeapon;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot Equip or Unequip"));
	}
}

void AARPGCharacter::LightAttack()
{
	if (CanAttack())
	{
		PlayLightAttackMontage();
		ActionState = EActionState::EAS_Attacking;
		EquippedWeapon->SetAttackDamage(EquippedWeapon->GetDamage());
	}
	
}

void AARPGCharacter::HeavyAttack()
{
	if (CanAttack())
	{
		PlayHeavyAttackMontage();
		ActionState = EActionState::EAS_Attacking;
		EquippedWeapon->SetAttackDamage(EquippedWeapon->GetDamage() * HeavyAttackMultiplier);

	}
}

bool AARPGCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}



bool AARPGCharacter::CanUnEquip()
{
	return ActionState == EActionState::EAS_Unoccupied
		&& CharacterState != ECharacterState::ECS_Unequipped;
}

bool AARPGCharacter::CanEquip()
{
	return ActionState == EActionState::EAS_Unoccupied
		&& CharacterState == ECharacterState::ECS_Unequipped 
		&& EquippedWeapon;
}

void AARPGCharacter::Unequip()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void AARPGCharacter::Equip()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}


void AARPGCharacter::PlayLightAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		int32 Selection = FMath::RandRange(0, 2);
		FName SectionName = FName();

		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		case 2:
			SectionName = FName("Attack3");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AARPGCharacter::PlayHeavyAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HeavyAttackMontage)
	{
		AnimInstance->Montage_Play(HeavyAttackMontage);
		int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();

		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, HeavyAttackMontage);
	}
}

void AARPGCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		// Play the montage and jump to the specific section (Equip/Unequip)
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}


void AARPGCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AARPGCharacter::FinishEquiping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AARPGCharacter::Jump()
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	Super::Jump();
}

void AARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AARPGCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AARPGCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AARPGCharacter::Jump);
		EnhancedInputComponent->BindAction(EActionMapping, ETriggerEvent::Triggered, this, &AARPGCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Triggered, this, &AARPGCharacter::LightAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &AARPGCharacter::HeavyAttack);
		EnhancedInputComponent->BindAction(EquipActionMapping, ETriggerEvent::Triggered, this, &AARPGCharacter::EquipKeyPressed);
	}
}


