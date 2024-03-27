// Fill out your copyright notice in the Description page of Project Settings.


#include "DDSoulShopWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
//My classes
#include "../UI/DDShopButton.h"
#include "../UI/DDUpgradeShopButton.h"

TArray<UDDShopButton*> UDDSoulShopWidget::FindAllShopButtons(UCanvasPanel* Canvas)
{
    TArray<UDDShopButton*> ButtonArray;
    RecursiveFindButtons(Canvas, ButtonArray);
    UE_LOG(LogTemp, Log, TEXT("Button amount: %d"), ButtonArray.Num());
    return ButtonArray;
}

void UDDSoulShopWidget::UpdateButtonsOfSameCategory(UDDUpgradeShopButton* Button)
{
    EPlayerStats Stat = Button->GetStatUpgrade();
    if (UpgradeButtonsByCategory.Find(Stat) != nullptr) {
        TArray<UDDUpgradeShopButton*> ButtonArray = UpgradeButtonsByCategory[Stat];
        for (UDDUpgradeShopButton* TempButton : ButtonArray) {
            if (Button == TempButton) {
                continue;
            }

            TempButton->IncreasePrice();
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Stat not found in button map. Was the map initialized correctly?"))
    }
}

void UDDSoulShopWidget::AddToButtonMap(UDDUpgradeShopButton* Button)
{
    EPlayerStats Stat = Button->GetStatUpgrade();
    if (UpgradeButtonsByCategory.Find(Stat) == nullptr) {
        UpgradeButtonsByCategory.Add(Stat);
        UpgradeButtonsByCategory[Stat].Add(Button);
    }
    else {
        UpgradeButtonsByCategory[Stat].Add(Button);
    }
}

//chatgpt the goat for this one ngl
void UDDSoulShopWidget::RecursiveFindButtons(UWidget* Widget, TArray<UDDShopButton*>& ButtonArray)
{    
    if (Widget->IsA<UDDShopButton>()) {
        UDDShopButton* Button = Cast<UDDShopButton>(Widget);
        ButtonArray.Add(Button);
        if (Button->IsA<UDDUpgradeShopButton>()) {
            UDDUpgradeShopButton* UpButton = Cast<UDDUpgradeShopButton>(Button);
            AddToButtonMap(UpButton);
        }
    }
    else if (Widget->IsA<UPanelWidget>()) {
        UPanelWidget* Panel = Cast<UPanelWidget>(Widget);
        TArray<UWidget*> ChildWidgets = Panel->GetAllChildren();
        for (UWidget* ChildWidget : ChildWidgets) {
            //Recursion in this day and age? How shocking!
            RecursiveFindButtons(ChildWidget, ButtonArray);
        }
    }
}

