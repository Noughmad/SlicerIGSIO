/*==============================================================================

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Kyle Sunderland, PerkLab, Queen's University
  and was supported through CANARIE's Research Software Program, and Cancer
  Care Ontario.

==============================================================================*/

#include <vtkMRMLVolumeReconstructionNode.h>

// MRML includes
#include <vtkMRMLAnnotationROINode.h>
#include <vtkMRMLScene.h>
#include <vtkMRMLVolumeNode.h>

// Sequnce MRML includes
#include <vtkMRMLSequenceBrowserNode.h>

// VTK includes
#include <vtkCommand.h>

//----------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLVolumeReconstructionNode);

//----------------------------------------------------------------------------
vtkMRMLVolumeReconstructionNode::vtkMRMLVolumeReconstructionNode()
{
  this->LiveVolumeReconstruction = false;

  this->LiveUpdateIntervalSeconds = 1.0;

  this->ClipRectangleOrigin[0] = 0;
  this->ClipRectangleOrigin[1] = 0;

  this->ClipRectangleSize[0] = 0;
  this->ClipRectangleSize[1] = 0;

  this->OutputSpacing[0] = 1.0;
  this->OutputSpacing[1] = 1.0;
  this->OutputSpacing[2] = 1.0;

  this->InterpolationMode = 0;
  this->OptimizationMode = 0;
  this->CompoundingMode = 0;
  this->FillHoles = false;
  this->NumberOfThreads = 0;

  this->NumberOfVolumesAddedToReconstruction = 0;
  this->LiveVolumeReconstructionInProgress = false;

  this->AddNodeReferenceRole(this->GetInputSequenceBrowserNodeReferenceRole(), this->GetInputSequenceBrowserNodeReferenceMRMLAttributeName());
  this->AddNodeReferenceRole(this->GetInputROINodeReferenceRole(), this->GetInputROINodeReferenceMRMLAttributeName());
  this->AddNodeReferenceRole(this->GetOutputVolumeNodeReferenceRole(), this->GetOutputVolumeNodeReferenceMRMLAttributeName());

  vtkNew<vtkIntArray> inputVolumeEvents;
  inputVolumeEvents->InsertNextTuple1(vtkCommand::ModifiedEvent);
  inputVolumeEvents->InsertNextTuple1(vtkMRMLTransformableNode::TransformModifiedEvent);
  this->AddNodeReferenceRole(this->GetInputVolumeNodeReferenceRole(), this->GetInputVolumeNodeReferenceMRMLAttributeName(), inputVolumeEvents);
}

//----------------------------------------------------------------------------
vtkMRMLVolumeReconstructionNode::~vtkMRMLVolumeReconstructionNode()
{
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeReconstructionNode::WriteXML(ostream& of, int nIndent)
{
  Superclass::WriteXML(of, nIndent);
  vtkMRMLWriteXMLBeginMacro(of);
  vtkMRMLWriteXMLBooleanMacro(liveVolumeReconstruction, LiveVolumeReconstruction);
  vtkMRMLWriteXMLFloatMacro(liveUpdateIntervalSeconds, LiveUpdateIntervalSeconds);
  vtkMRMLWriteXMLVectorMacro(clipRectangleOrigin, ClipRectangleOrigin, int, 2);
  vtkMRMLWriteXMLVectorMacro(clipRectangleSize, ClipRectangleSize, int, 2);
  vtkMRMLWriteXMLVectorMacro(outputSpacing, OutputSpacing, double, 3);
  vtkMRMLWriteXMLIntMacro(interpolationMode, InterpolationMode);
  vtkMRMLWriteXMLIntMacro(optimizationMode, OptimizationMode);
  vtkMRMLWriteXMLIntMacro(compoundingMode, CompoundingMode);
  vtkMRMLWriteXMLBooleanMacro(fillHoles, FillHoles);
  vtkMRMLWriteXMLIntMacro(numberOfThreads, NumberOfThreads);
  vtkMRMLWriteXMLEndMacro();
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeReconstructionNode::ReadXMLAttributes(const char** atts)
{
  MRMLNodeModifyBlocker blocker(this);
  Superclass::ReadXMLAttributes(atts);
  vtkMRMLReadXMLBeginMacro(atts);
  vtkMRMLReadXMLBooleanMacro(liveVolumeReconstruction, LiveVolumeReconstruction);
  vtkMRMLReadXMLFloatMacro(liveUpdateIntervalSeconds, LiveUpdateIntervalSeconds);
  vtkMRMLReadXMLVectorMacro(clipRectangleOrigin, ClipRectangleOrigin, int, 2);
  vtkMRMLReadXMLVectorMacro(clipRectangleSize, ClipRectangleSize, int, 2);
  vtkMRMLReadXMLVectorMacro(outputSpacing, OutputSpacing, double, 3);
  vtkMRMLReadXMLIntMacro(interpolationMode, InterpolationMode);
  vtkMRMLReadXMLIntMacro(optimizationMode, OptimizationMode);
  vtkMRMLReadXMLIntMacro(compoundingMode, CompoundingMode);
  vtkMRMLReadXMLBooleanMacro(fillHoles, FillHoles);
  vtkMRMLReadXMLIntMacro(numberOfThreads, NumberOfThreads);
  vtkMRMLReadXMLEndMacro();
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeReconstructionNode::Copy(vtkMRMLNode* anode)
{
  MRMLNodeModifyBlocker blocker(this);
  Superclass::Copy(anode);
  this->DisableModifiedEventOn();
  vtkMRMLCopyBeginMacro(anode);
  vtkMRMLCopyBooleanMacro(LiveVolumeReconstruction);
  vtkMRMLCopyFloatMacro(LiveUpdateIntervalSeconds);
  vtkMRMLCopyVectorMacro(ClipRectangleOrigin, int, 2);
  vtkMRMLCopyVectorMacro(ClipRectangleSize, int, 2);
  vtkMRMLCopyVectorMacro(OutputSpacing, double, 3);
  vtkMRMLCopyIntMacro(InterpolationMode);
  vtkMRMLCopyIntMacro(OptimizationMode);
  vtkMRMLCopyIntMacro(CompoundingMode);
  vtkMRMLCopyBooleanMacro(FillHoles);
  vtkMRMLCopyIntMacro(NumberOfThreads);
  vtkMRMLCopyEndMacro();
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeReconstructionNode::PrintSelf(ostream& os, vtkIndent indent)
{
  Superclass::PrintSelf(os, indent);
  vtkMRMLPrintBeginMacro(os, indent);
  vtkMRMLPrintBooleanMacro(LiveVolumeReconstruction);
  vtkMRMLPrintFloatMacro(LiveUpdateIntervalSeconds);
  vtkMRMLPrintFloatMacro(LiveUpdateIntervalSeconds);
  vtkMRMLPrintVectorMacro(ClipRectangleOrigin, int, 2);
  vtkMRMLPrintVectorMacro(ClipRectangleSize, int, 2);
  vtkMRMLPrintVectorMacro(OutputSpacing, double, 3);
  vtkMRMLPrintIntMacro(InterpolationMode);
  vtkMRMLPrintIntMacro(OptimizationMode);
  vtkMRMLPrintIntMacro(CompoundingMode);
  vtkMRMLPrintBooleanMacro(FillHoles);
  vtkMRMLPrintIntMacro(NumberOfThreads);
  vtkMRMLPrintIntMacro(NumberOfVolumesAddedToReconstruction);
  vtkMRMLPrintIntMacro(LiveVolumeReconstructionInProgress);
  vtkMRMLPrintEndMacro();
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeReconstructionNode::ProcessMRMLEvents(vtkObject* caller, unsigned long eventID, void* callData)
{
  Superclass::ProcessMRMLEvents(caller, eventID, callData);
  if (!this->Scene)
  {
    vtkErrorMacro("ProcessMRMLEvents: Invalid MRML scene");
    return;
  }

  vtkMRMLVolumeNode* volumeNode = vtkMRMLVolumeNode::SafeDownCast(caller);
  if (volumeNode && volumeNode == this->GetInputVolumeNode())
  {
    this->InvokeEvent(InputVolumeModified, volumeNode);
  }
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeReconstructionNode::SetNumberOfVolumesAddedToReconstruction(int numberOfVolumesAddedToReconstruction)
{
  this->NumberOfVolumesAddedToReconstruction = numberOfVolumesAddedToReconstruction;
};

//----------------------------------------------------------------------------
void vtkMRMLVolumeReconstructionNode::SetAndObserveInputSequenceBrowserNode(vtkMRMLSequenceBrowserNode* node)
{
  this->SetNodeReferenceID(this->GetInputSequenceBrowserNodeReferenceRole(), (node ? node->GetID() : NULL));
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeReconstructionNode::SetAndObserveInputVolumeNode(vtkMRMLVolumeNode* node)
{
  this->SetNodeReferenceID(this->GetInputVolumeNodeReferenceRole(), (node ? node->GetID() : NULL));
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeReconstructionNode::SetAndObserveInputROINode(vtkMRMLAnnotationROINode* node)
{
  this->SetNodeReferenceID(this->GetInputROINodeReferenceRole(), (node ? node->GetID() : NULL));
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeReconstructionNode::SetAndObserveOutputVolumeNode(vtkMRMLVolumeNode* node)
{
  this->SetNodeReferenceID(this->GetOutputVolumeNodeReferenceRole(), (node ? node->GetID() : NULL));
}

//----------------------------------------------------------------------------
vtkMRMLSequenceBrowserNode* vtkMRMLVolumeReconstructionNode::GetInputSequenceBrowserNode()
{
  return vtkMRMLSequenceBrowserNode::SafeDownCast(this->GetNodeReference(this->GetInputSequenceBrowserNodeReferenceRole()));
}

//----------------------------------------------------------------------------
vtkMRMLVolumeNode* vtkMRMLVolumeReconstructionNode::GetInputVolumeNode()
{
  return vtkMRMLVolumeNode::SafeDownCast(this->GetNodeReference(this->GetInputVolumeNodeReferenceRole()));
}

//----------------------------------------------------------------------------
vtkMRMLAnnotationROINode* vtkMRMLVolumeReconstructionNode::GetInputROINode()
{
  return vtkMRMLAnnotationROINode::SafeDownCast(this->GetNodeReference(this->GetInputROINodeReferenceRole()));
}

//----------------------------------------------------------------------------
vtkMRMLVolumeNode* vtkMRMLVolumeReconstructionNode::GetOutputVolumeNode()
{
  return vtkMRMLVolumeNode::SafeDownCast(this->GetNodeReference(this->GetOutputVolumeNodeReferenceRole()));
}