// Fill out your copyright notice in the Description page of Project Settings.

#include "Topologies.h"
#include "LeapNetUE4.h"

UTopologies::UTopologies() {
	bPrevSaved = false;
	Topologies.Empty();
	UE_LOG(LogTemp, Warning, TEXT("UTopologies save file constructed"));
}



FTopology UTopologies::GetTopologyFromName(FString topologyName) {
	for (int i = 0; i < Topologies.Num(); i++) {
		if (Topologies[i].topologyName == topologyName) {
			return Topologies[i];
		}
	}

	

	return Topologies[0];
	
}

// Checks if there is a topology given a name. Mostly used to ensure no errors occur in the use of the system
bool UTopologies::DoesTopologyExist(FString topologyName) {
	UE_LOG(LogTemp, Warning, TEXT("Name to check for: %s"), *topologyName);
	FString testMess = this->bPrevSaved ? "Has been Saved" : "Has not been Saved";
	UE_LOG(LogTemp, Warning, TEXT("%s"), *testMess);
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