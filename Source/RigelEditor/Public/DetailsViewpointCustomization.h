// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"


class FDetailsViewpointCustomization : public IDetailCustomization
{
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    static TSharedRef<IDetailCustomization> MakeInstance();

    void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

public:

    FReply OnAddViewpoint();
private:
};
