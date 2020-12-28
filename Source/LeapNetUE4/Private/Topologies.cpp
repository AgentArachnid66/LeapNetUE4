// Fill out your copyright notice in the Description page of Project Settings.

#include "LeapNetUE4.h"
#include "Topologies.h"




FTopology UTopologies::GetTopologyFromName(FString topologyName) {
	for (int i = 0; i < Topologies.Num(); i++) {
		if (Topologies[i].topologyName == topologyName) {
			return Topologies[i];
		}
	}

	return Topologies[0];
	
}

bool UTopologies::DoesTopologyExist(FString topologyName) {
	UE_LOG(LogTemp, Warning, TEXT("Name to check for: %s"), *topologyName);
	for (int i = 0; i < Topologies.Num(); i++) {
		UE_LOG(LogTemp, Warning, TEXT("Current Topology Name: %s"), *Topologies[i].topologyName);
		if (Topologies[i].topologyName == topologyName) {
			return true;
		}
	}
	return false;
}

int UTopologies::GetTopologyIndexFromName(FString topologyName) {
	for (int i = 0; i < Topologies.Num(); i++) {
		if (Topologies[i].topologyName == topologyName) {
			return i;
		}
	}
	return 0;
}