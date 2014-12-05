
#ifndef _FabricSpliceBaseInterface_H_
#define _FabricSpliceBaseInterface_H_

#include "FabricSpliceConversion.h"
#include "plugin.h"

#include <vector>

#include <maya/MFnDependencyNode.h> 
#include <maya/MPlug.h> 
#include <maya/MPxNode.h> 
#include <maya/MTypeId.h> 
#include <maya/MNodeMessage.h>
#include <maya/MStringArray.h>
#include <maya/MFnCompoundAttribute.h>

#include <FabricSplice.h>

#define MAYASPLICE_CATCH_BEGIN(statusPtr) \
  if(statusPtr) \
    *statusPtr=MS::kSuccess; \
  try {

#define MAYASPLICE_CATCH_END(statusPtr) } \
  catch (FabricSplice::Exception e) { \
    mayaLogErrorFunc(e.what()); \
    if (statusPtr) \
      *statusPtr=MS::kFailure; \
  } \
  catch (FabricCore::Exception e) { \
    mayaLogErrorFunc(e.getDesc_cstr()); \
    if (statusPtr) \
      *statusPtr=MS::kFailure; \
  }

class FabricSpliceBaseInterface {

public:

  FabricSpliceBaseInterface();
  virtual ~FabricSpliceBaseInterface();
  virtual void constructBaseInterface();

  virtual MObject getThisMObject() = 0;
  virtual MPlug getSaveDataPlug() = 0;

  static std::vector<FabricSpliceBaseInterface*> getInstances();
  static FabricSpliceBaseInterface * getInstanceByName(const std::string & name);

  virtual MObject addMayaAttribute(const MString &portName, const MString &dataType, const MString &arrayType, const FabricSplice::Port_Mode &portMode, bool compoundChild = false, FabricCore::Variant compoundStructure = FabricCore::Variant(), MStatus *stat = 0);
  virtual void addPort(const MString &portName, const MString &dataType, const FabricSplice::Port_Mode &portMode, const MString & dgNode, bool autoInitObjects, const MString & extension, const FabricCore::Variant & defaultValue, MStatus *stat = 0);
  virtual void removeMayaAttribute(const MString &portName, MStatus *stat = 0);
  virtual void removePort(const MString &portName, MStatus *stat = 0);
  virtual void addKLOperator(const MString &operatorName, const MString &operatorCode, const MString &operatorEntry, const MString & dgNode, const FabricCore::Variant & portMap, MStatus *stat = 0);
  virtual void setKLOperatorEntry(const MString &operatorName, const MString &operatorEntry, MStatus *stat = 0);
  virtual void setKLOperatorIndex(const MString &operatorName, unsigned int operatorIndex, MStatus *stat = 0);
  virtual void setKLOperatorCode(const MString &operatorName, const MString &operatorCode, const MString &operatorEntry, MStatus *stat = 0);
  virtual std::string getKLOperatorCode(const MString &operatorName, MStatus *stat = 0);
  virtual void setKLOperatorFile(const MString &operatorName, const MString &filename, const MString &entry, MStatus *stat = 0);
  virtual void removeKLOperator(const MString &operatorName, const MString & dgNode, MStatus *stat = 0);
  virtual void storePersistenceData(MString file, MStatus *stat = 0);
  virtual void restoreFromPersistenceData(MString file, MStatus *stat = 0);
  virtual void resetInternalData(MStatus *stat = 0);
  virtual MStringArray getKLOperatorNames();
  virtual MStringArray getPortNames();
  virtual FabricSplice::DGPort getPort(MString name);
  virtual void saveToFile(MString fileName);
  virtual MStatus loadFromFile(MString fileName);
  virtual void setPortPersistence(const MString &portName, bool persistence);
  virtual FabricSplice::DGGraph & getSpliceGraph() { return _spliceGraph; }
  virtual void setDgDirtyEnabled(bool enabled) { _dgDirtyEnabled = enabled; }

  static void onNodeAdded(MObject &node, void *clientData);
  static void onNodeRemoved(MObject &node, void *clientData);

  virtual void managePortObjectValues(bool destroy);

protected:

  virtual void invalidatePlug(MPlug & plug);
  virtual void invalidateNode();
  virtual void setupMayaAttributeAffects(MString portName, FabricSplice::Port_Mode portMode, MObject newAttribute, MStatus *stat = 0);
  virtual void transferInputValuesToSplice(MDataBlock& data);
  virtual void clearDirtyPlugs();
  virtual void evaluate();
  virtual void transferOutputValuesToMaya(MDataBlock& data, bool isDeformer = false);
  virtual void collectDirtyPlug(MPlug const &inPlug);
  virtual void affectChildPlugs(MPlug &plug, MPlugArray &affectedPlugs);
  virtual void setDependentsDirty(MObject thisMObject, MPlug const &inPlug, MPlugArray &affectedPlugs);
  virtual void copyInternalData(MPxNode *node);

  FabricSplice::DGGraph _spliceGraph;
  bool _outputsDirtied;

private:
  
  // private members and helper methods
  static std::vector<FabricSpliceBaseInterface*> _instances;
  bool _restoredFromPersistenceData;
  unsigned int _dummyValue;

  MStringArray _dirtyPlugs;
  MStringArray _evalContextPlugNames;
  MIntArray _evalContextPlugIds;
  std::vector<std::string> mSpliceMayaDataOverride;
  bool _isTransferingInputs;
  bool _portObjectsDestroyed;


  // static MString sManipulationCommand;
  // MString _manipulationCommand;
  bool _dgDirtyEnabled;
  bool _affectedPlugsDirty;
  MPlugArray _affectedPlugs;

#if _SPLICE_MAYA_VERSION < 2014
  static std::map<std::string, int> _nodeCreatorCounts;
#endif

  bool plugInArray(const MPlug &plug, const MPlugArray &array);
};

#endif
