#include "DeadParadiseEditor.h"

#include "PathfindingComponentVisualizer.h"
#include "DeadParadise/PathfindingGrid.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FDeadParadiseEditor, DeadParadiseEditor)

DEFINE_LOG_CATEGORY(DeadParadiseEditor)

#define LOCTEXT_NAMESPACE "DeadParadiseEditor"

void FDeadParadiseEditor::StartupModule()
{
	UE_LOG(DeadParadiseEditor, Display, TEXT("DeadParadiseEditor: Log Started"));

	if (!GUnrealEd) return;
	
	TSharedPtr<FPathfindingComponentVisualizer> PathfinderVisualizer = MakeShareable(new FPathfindingComponentVisualizer());
	if (PathfinderVisualizer.IsValid()) {
		GUnrealEd->RegisterComponentVisualizer(UPathfindingGrid::StaticClass()->GetFName(), PathfinderVisualizer);
			
		UE_LOG(DeadParadiseEditor, Display, TEXT("DeadParadiseEditor: PathfindingGrid Visualizer registerd"));
	}
		
	UE_LOG(DeadParadiseEditor, Error, TEXT("DeadParadiseEditor: PathfindingGrid Visualizer is Invalid"));
}

void FDeadParadiseEditor::ShutdownModule()
{
	if (!GUnrealEd) return;
	
	GUnrealEd->UnregisterComponentVisualizer(UPathfindingGrid::StaticClass()->GetFName());
	
	UE_LOG(DeadParadiseEditor, Display, TEXT("DeadParadiseEditor: Log Ended"));
}

#undef LOCTEXT_NAMESPACE