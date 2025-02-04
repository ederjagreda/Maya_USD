//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#ifndef PXRUSDMAYA_JOB_ARGS_H
#define PXRUSDMAYA_JOB_ARGS_H

#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>

#include <pxr/pxr.h>
#include <pxr/base/tf/staticTokens.h>
#include <pxr/base/tf/token.h>
#include <pxr/usd/sdf/path.h>

#include <maya/MString.h>

#include <mayaUsd/base/api.h>
#include <mayaUsd/utils/util.h>

PXR_NAMESPACE_OPEN_SCOPE

#define PXRUSDMAYA_TRANSLATOR_TOKENS \
    ((UsdFileExtensionDefault, "usd")) \
    ((UsdFileExtensionASCII, "usda")) \
    ((UsdFileExtensionCrate, "usdc")) \
    ((UsdFileExtensionPackage, "usdz")) \
    ((UsdReadableFileFilter, "*.usd *.usda *.usdc *.usdz")) \
    ((UsdWritableFileFilter, "*.usd *.usda *.usdc *.usdz"))

TF_DECLARE_PUBLIC_TOKENS(
    UsdMayaTranslatorTokens,
    MAYAUSD_CORE_PUBLIC,
    PXRUSDMAYA_TRANSLATOR_TOKENS);

#define PXRUSDMAYA_JOB_EXPORT_ARGS_TOKENS \
    /* Dictionary keys */ \
    (chaser) \
    (chaserArgs) \
    (compatibility) \
    (defaultCameras) \
    (defaultMeshScheme) \
    (defaultUSDFormat) \
    (eulerFilter) \
    (exportCollectionBasedBindings) \
    (exportColorSets) \
    (exportDisplayColor) \
    (exportInstances) \
    (exportMaterialCollections) \
    (exportReferenceObjects) \
    (exportRefsAsInstanceable) \
    (exportSkels) \
    (exportSkin) \
    (exportUVs) \
    (exportVisibility) \
    (kind) \
    (materialCollectionsPath) \
    (materialsScopeName) \
    (melPerFrameCallback) \
    (melPostCallback) \
    (mergeTransformAndShape) \
    (normalizeNurbs) \
    (parentScope) \
    (pythonPerFrameCallback) \
    (pythonPostCallback) \
    (renderableOnly) \
    (renderLayerMode) \
    (shadingMode) \
    (stripNamespaces) \
    (verbose) \
    /* Special "none" token */ \
    (none) \
    /* renderLayerMode values */ \
    (defaultLayer) \
    (currentLayer) \
    (modelingVariant) \
    /* exportSkels/exportSkin values */ \
    ((auto_, "auto")) \
    ((explicit_, "explicit")) \
    /* compatibility values */ \
    (appleArKit)

TF_DECLARE_PUBLIC_TOKENS(
    UsdMayaJobExportArgsTokens,
    MAYAUSD_CORE_PUBLIC,
    PXRUSDMAYA_JOB_EXPORT_ARGS_TOKENS);

#define PXRUSDMAYA_JOB_IMPORT_ARGS_TOKENS \
    /* Dictionary keys */ \
    (apiSchema) \
    (assemblyRep) \
    (excludePrimvar) \
    (metadata) \
    (shadingMode) \
    (shadingConversion) \
    (useAsAnimationCache) \
    /* assemblyRep values */ \
    (Collapsed) \
    (Full) \
    (Import) \
    ((Unloaded, ""))

TF_DECLARE_PUBLIC_TOKENS(
    UsdMayaJobImportArgsTokens,
    MAYAUSD_CORE_PUBLIC,
    PXRUSDMAYA_JOB_IMPORT_ARGS_TOKENS);


struct UsdMayaJobExportArgs
{
    const TfToken compatibility;
    const TfToken defaultMeshScheme;
    const TfToken defaultUSDFormat;
    const bool eulerFilter;
    const bool excludeInvisible;

    /// If set to false, then direct per-gprim bindings are exported.
    /// If set to true and if \p materialCollectionsPath is non-empty, then
    /// material-collections are created and bindings are made to the
    /// collections at \p materialCollectionsPath, instead of direct
    /// per-gprim bindings.
    const bool exportCollectionBasedBindings;
    const bool exportColorSets;
    const bool exportDefaultCameras;
    const bool exportDisplayColor;
    const bool exportInstances;
    const bool exportMaterialCollections;
    const bool exportMeshUVs;
    const bool exportNurbsExplicitUV;
    const bool exportReferenceObjects;
    const bool exportRefsAsInstanceable;
    const TfToken exportSkels;
    const TfToken exportSkin;
    const bool exportVisibility;

    /// If this is not empty, then a set of collections are exported on the
    /// prim pointed to by the path, each representing the collection of
    /// geometry that's bound to the various shading group sets in Maya.
    const SdfPath materialCollectionsPath;

    /// This is the name of the USD prim under which material prims will be
    /// authored.
    const TfToken materialsScopeName;

    /// Whether the transform node and the shape node must be merged into
    /// a single node in the output USD.
    const bool mergeTransformAndShape;
    const bool normalizeNurbs;
    const bool stripNamespaces;

    /// This is the path of the USD prim under which *all* prims will be
    /// authored.
    const SdfPath parentScope;
    const TfToken renderLayerMode;
    const TfToken rootKind;
    const TfToken shadingMode;
    const bool verbose;

    typedef std::map<std::string, std::string> ChaserArgs;
    const std::vector<std::string> chaserNames;
    const std::map< std::string, ChaserArgs > allChaserArgs;

    const std::string melPerFrameCallback;
    const std::string melPostCallback;
    const std::string pythonPerFrameCallback;
    const std::string pythonPostCallback;

    const UsdMayaUtil::MDagPathSet dagPaths;
    /// The time samples at which to export animated data; the times must be
    /// monotonically non-decreasing.
    /// An empty list of time samples means that no animated (time-sampled)
    /// data should be exported.
    const std::vector<double> timeSamples;

    // This path is provided when dealing with variants
    // where a _BaseModel_ root path is used instead of
    // the model path. This to allow a proper internal reference.
    SdfPath usdModelRootOverridePath; // XXX can we make this const?

    /// Creates a UsdMayaJobExportArgs from the given \p dict, overlaid on
    /// top of the default dictionary given by GetDefaultDictionary().
    /// The values of \p dict are stronger (will override) the values from the
    /// default dictionary.
    /// Issues runtime errors if \p dict contains values of the wrong type;
    /// types should match those declared in GetDefaultDictionary().
    MAYAUSD_CORE_PUBLIC
    static UsdMayaJobExportArgs CreateFromDictionary(
        const VtDictionary& userArgs,
        const UsdMayaUtil::MDagPathSet& dagPaths,
        const std::vector<double>& timeSamples = std::vector<double>());

    /// Gets the default arguments dictionary for UsdMayaJobExportArgs.
    MAYAUSD_CORE_PUBLIC
    static const VtDictionary& GetDefaultDictionary();

    /// Adds type name to filter out during export. This will also add all
    /// inherited types (so if you exclude "constraint", it will also exclude
    /// "parentConstraint")
    MAYAUSD_CORE_PUBLIC
    void AddFilteredTypeName(const MString& typeName);

    const std::set<unsigned int>& GetFilteredTypeIds() const {
        return _filteredTypeIds;
    }

    void ClearFilteredTypeIds() {
        _filteredTypeIds.clear();
    }

private:
    MAYAUSD_CORE_PUBLIC
    UsdMayaJobExportArgs(
        const VtDictionary& userArgs,
        const UsdMayaUtil::MDagPathSet& dagPaths,
        const std::vector<double>& timeSamples = std::vector<double>());

    // Maya type ids to avoid exporting; these are
    // EXACT types, though the only exposed way to modify this,
    // AddFilteredTypeName, will also add all inherited types
    // (so if you exclude "constraint", it will also exclude
    // "parentConstraint")
    std::set<unsigned int> _filteredTypeIds;
};

MAYAUSD_CORE_PUBLIC
std::ostream& operator <<(
    std::ostream& out,
    const UsdMayaJobExportArgs& exportArgs);


struct UsdMayaJobImportArgs
{
    const TfToken assemblyRep;
    const TfToken::Set excludePrimvarNames;
    const TfToken::Set includeAPINames;
    const TfToken::Set includeMetadataKeys;
    TfToken shadingMode; // XXX can we make this const?
    const TfToken shadingConversion;
    const bool useAsAnimationCache;

    const bool importWithProxyShapes;
    /// The interval over which to import animated data.
    /// An empty interval (<tt>GfInterval::IsEmpty()</tt>) means that no
    /// animated (time-sampled) data should be imported.
    /// A full interval (<tt>timeInterval == GfInterval::GetFullInterval()</tt>)
    /// means to import all available data, though this does not need to be
    /// special-cased because USD will accept full intervals like any other
    /// non-empty interval.
    const GfInterval timeInterval;

    /// Creates a UsdMayaJobImportArgs from the given \p dict, overlaid on
    /// top of the default dictionary given by GetDefaultDictionary().
    /// The values of \p dict are stronger (will override) the values from the
    /// default dictionary.
    /// Issues runtime errors if \p dict contains values of the wrong type;
    /// types should match those declared in GetDefaultDictionary().
    MAYAUSD_CORE_PUBLIC
    static UsdMayaJobImportArgs CreateFromDictionary(
        const VtDictionary& userArgs,
        const bool importWithProxyShapes = false,
        const GfInterval& timeInterval = GfInterval::GetFullInterval());

    /// Gets the default arguments dictionary for UsdMayaJobImportArgs.
    MAYAUSD_CORE_PUBLIC
    static const VtDictionary& GetDefaultDictionary();

private:
    MAYAUSD_CORE_PUBLIC
    UsdMayaJobImportArgs(
        const VtDictionary& userArgs,
        const bool importWithProxyShapes,
        const GfInterval& timeInterval);
};

MAYAUSD_CORE_PUBLIC
std::ostream& operator <<(
    std::ostream& out,
    const UsdMayaJobImportArgs& importArgs);


PXR_NAMESPACE_CLOSE_SCOPE


#endif
