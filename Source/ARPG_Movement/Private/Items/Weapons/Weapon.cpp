// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/ARPGCharacter.h"

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	AARPGCharacter* ARPGCharacter = Cast<AARPGCharacter>(OtherActor);
	if (ARPGCharacter) 
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		ItemMesh->AttachToComponent(ARPGCharacter->GetMesh(), TransformRules, FName("RightHandSocket"));
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}
