// Fill out your copyright notice in the Description page of Project Settings.


#include "DDActorLibrary.h"

AActor* UDDActorLibrary::SpawnDeferredActorFromClass(UObject* WorldContextObject, UClass* Class, const FTransform &Transform) 
{
	if (!WorldContextObject) {
		return nullptr;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) {
		return nullptr;
	}

	return World->SpawnActorDeferred<AActor>(Class, Transform);
}
