// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/PrototypeEquipmentDefinition.h"

#include "Equipment/PrototypeEquipmentInstance.h"

UPrototypeEquipmentDefinition::UPrototypeEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
{
	InstanceType = UPrototypeEquipmentInstance::StaticClass();
}
