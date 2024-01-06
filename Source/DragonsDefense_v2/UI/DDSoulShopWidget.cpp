// Fill out your copyright notice in the Description page of Project Settings.


#include "DDSoulShopWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
//My classes
#include "../UI/DDShopButton.h"

TArray<UDDShopButton*> UDDSoulShopWidget::FindAllShopButtons(UCanvasPanel* Canvas)
{
    TArray<UDDShopButton*> ButtonArray;
    RecursiveFindButtons(Canvas, ButtonArray);
    UE_LOG(LogTemp, Log, TEXT("Button amount: %d"), ButtonArray.Num());
    return ButtonArray;
}

void UDDSoulShopWidget::RecursiveFindButtons(UWidget* Widget, TArray<UDDShopButton*>& ButtonArray)
{    
    UDDShopButton* Button = Cast<UDDShopButton>(Widget);
    if (Button) {
        ButtonArray.Add(Button);
    }
    else if (UPanelWidget* Panel = Cast<UPanelWidget>(Widget)) {
        TArray<UWidget*> ChildWidgets = Panel->GetAllChildren();
        for (UWidget* ChildWidget : ChildWidgets) {
            //Recursion in this day and age? How shocking!
            RecursiveFindButtons(ChildWidget, ButtonArray);
        }
    }
}

