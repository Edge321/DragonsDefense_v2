// Fill out your copyright notice in the Description page of Project Settings.


#include "DDColliderLibrary.h"
#include "Components/ShapeComponent.h"

void DDColliderLibrary::SetCollisionChannelToIgnore(UShapeComponent* Collider, const ECollisionChannel Channel)
{
	Collider->SetCollisionResponseToChannel(Channel, ECR_Ignore);
}
