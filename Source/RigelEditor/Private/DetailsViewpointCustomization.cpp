#include "DetailsViewpointCustomization.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "Widgets/Input/SButton.h"
#include "PropertyEditorModule.h"
#include "DetailWidgetRow.h"
#include "Components/ViewpointComponent.h"
#include "Widgets/Text/STextBlock.h"
#include "Internationalization/Internationalization.h"

#define LOCTEXT_NAMESPACE "DetailsViewpointCustomization"

TSharedRef<IDetailCustomization> FDetailsViewpointCustomization::MakeInstance()
{
    return MakeShareable(new FDetailsViewpointCustomization);
}

void FDetailsViewpointCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    IDetailCategoryBuilder& builder = DetailBuilder.EditCategory(TEXT("Rigel"));
    builder.AddCustomRow(LOCTEXT("Viewpoint AA", "aaa"))
        .ValueWidget
        [
            SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .FillWidth(1.0f)
                [
                    SNew(SButton)
                        .Text(LOCTEXT("Add Viewpoint", "添加当前位置"))
                        .OnClicked(this, &FDetailsViewpointCustomization::OnAddViewpoint)
                ]
            
        ];
    builder.AddProperty(GET_MEMBER_NAME_CHECKED(UViewpointComponent, ViewpointList));
}

FReply FDetailsViewpointCustomization::OnAddViewpoint()
{

    return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE