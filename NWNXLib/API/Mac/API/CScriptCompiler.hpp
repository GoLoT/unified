#pragma once

#include <cstdint>

#include "CExoArrayListTemplatedCExoString.hpp"
#include "CExoArrayListTemplatedint.hpp"
#include "CExoString.hpp"
#include "CScriptCompilerIncludeFileStackEntry.hpp"

namespace NWNXLib {

namespace API {

// Forward class declarations (defined in the source file)
struct CExoStringList;
struct CScriptCompilerIdListEntry;
struct CScriptCompilerIdentifierHashTableEntry;
struct CScriptCompilerKeyWordEntry;
struct CScriptCompilerStackEntry;
struct CScriptCompilerStructureEntry;
struct CScriptCompilerStructureFieldEntry;
struct CScriptCompilerSymbolTableEntry;
struct CScriptCompilerVarStackEntry;
struct CScriptParseTreeNode;
struct CScriptParseTreeNodeBlock;

struct CScriptCompiler
{
    int32_t m_nKeyWords;
    CScriptCompilerKeyWordEntry* m_pcKeyWords;
    int32_t m_nParseTreeNodeBlockEmptyNodes;
    CScriptParseTreeNodeBlock* m_pCurrentParseTreeNodeBlock;
    CScriptParseTreeNodeBlock* m_pParseTreeNodeBlockHead;
    CScriptParseTreeNodeBlock* m_pParseTreeNodeBlockTail;
    CExoString** m_ppsParseTreeFileNames;
    int32_t m_nNextParseTreeFileName;
    int32_t m_nCurrentParseTreeFileName;
    int32_t m_nCurrentLineNumber;
    int32_t m_nCurrentLineNumberFileReference;
    int32_t m_nCurrentLineNumberReferences;
    int32_t m_nCurrentLineNumberBinaryStartInstruction;
    int32_t m_nCurrentLineNumberBinaryEndInstruction;
    int32_t m_nTableFileNames;
    CExoString m_psTableFileNames[128];
    int32_t m_nLineNumberEntries;
    int32_t m_nFinalLineNumberEntries;
    CExoArrayListTemplatedint m_pnTableInstructionFileReference;
    CExoArrayListTemplatedint m_pnTableInstructionLineNumber;
    CExoArrayListTemplatedint m_pnTableInstructionBinaryStart;
    CExoArrayListTemplatedint m_pnTableInstructionBinaryEnd;
    CExoArrayListTemplatedint m_pnTableInstructionBinaryFinal;
    CExoArrayListTemplatedint m_pnTableInstructionBinarySortedOrder;
    int32_t m_nSymbolTableVariables;
    int32_t m_nFinalSymbolTableVariables;
    CExoArrayListTemplatedint m_pnSymbolTableVarType;
    CExoArrayListTemplatedCExoString m_psSymbolTableVarName;
    CExoArrayListTemplatedCExoString m_psSymbolTableVarStructureName;
    CExoArrayListTemplatedint m_pnSymbolTableVarStackLoc;
    CExoArrayListTemplatedint m_pnSymbolTableVarBegin;
    CExoArrayListTemplatedint m_pnSymbolTableVarEnd;
    CExoArrayListTemplatedint m_pnSymbolTableBinaryFinal;
    CExoArrayListTemplatedint m_pnSymbolTableBinarySortedOrder;
    int32_t m_nDebugStatus;
    int32_t m_bCompileConditionalFile;
    int32_t m_bOldCompileConditionalFile;
    int32_t m_bCompileConditionalOrMain;
    CExoString m_sLanguageSource;
    CExoString m_sOutputAlias;
    int32_t m_nLines;
    int32_t m_nCharacterOnLine;
    int32_t* m_pnHashString;
    CScriptCompilerIdentifierHashTableEntry* m_pIdentifierHashTable;
    int32_t m_nTokenStatus;
    int32_t m_nTokenCharacters;
    char m_pchToken[512];
    CScriptCompilerStackEntry* m_pSRStack;
    int32_t m_nSRStackEntries;
    int32_t m_nSRStackStates;
    int32_t m_bCompileIdentifierList;
    int32_t m_bCompileIdentifierConstants;
    int32_t m_nIdentifierListState;
    int32_t m_nIdentifierListVector;
    int32_t m_nIdentifierListEngineStructure;
    int32_t m_nIdentifierListReturnType;
    CScriptCompilerIdListEntry* m_pcIdentifierList;
    int32_t m_nOccupiedIdentifiers;
    int32_t m_nMaxPredefinedIdentifierId;
    int32_t m_nPredefinedIdentifierOrder;
    int32_t m_nCompileFileLevel;
    CScriptCompilerIncludeFileStackEntry m_pcIncludeFileStack[16];
    int32_t m_nVarStackRecursionLevel;
    CScriptCompilerVarStackEntry* m_pcVarStackList;
    int32_t m_nOccupiedVariables;
    int32_t m_nVarStackVariableType;
    CExoString m_sVarStackVariableTypeName;
    CScriptCompilerStructureEntry* m_pcStructList;
    CScriptCompilerStructureFieldEntry* m_pcStructFieldList;
    int32_t m_nMaxStructures;
    int32_t m_nMaxStructureFields;
    int32_t m_nStructureDefinition;
    int32_t m_nStructureDefinitionFieldStart;
    int32_t m_bGlobalVariableDefinition;
    int32_t m_nGlobalVariables;
    int32_t m_nGlobalVariableSize;
    CScriptParseTreeNode* m_pGlobalVariableParseTree;
    int32_t m_bConstantVariableDefinition;
    int32_t m_nLoopIdentifier;
    int32_t m_nLoopStackDepth;
    int32_t m_nSwitchLevel;
    int32_t m_nSwitchIdentifier;
    int32_t m_nSwitchStackDepth;
    CExoString m_sUndefinedIdentifier;
    int32_t m_bSwitchLabelDefault;
    int32_t m_nSwitchLabelNumber;
    int32_t m_nSwitchLabelArraySize;
    int32_t* m_pnSwitchLabelStatements;
    int32_t m_nNumEngineDefinedStructures;
    int32_t* m_pbEngineDefinedStructureValid;
    CExoString* m_psEngineDefinedStructureName;
    int32_t m_bAssignmentToVariable;
    int32_t m_bInStructurePart;
    int32_t m_bFunctionImp;
    CExoString m_sFunctionImpName;
    int32_t m_nFunctionImpReturnType;
    CExoString m_sFunctionImpReturnStructureName;
    int32_t m_nFunctionImpAbortStackPointer;
    int32_t m_nStackCurrentDepth;
    char m_pchStackTypes[8192];
    int32_t m_nRunTimeIntegers;
    int32_t m_nRunTimeFloats;
    int32_t m_nRunTimeStrings;
    int32_t m_nRunTimeObjects;
    int32_t m_nRunTimeActions;
    int32_t m_nSymbolQueryListSize;
    int32_t m_nSymbolQueryList;
    CScriptCompilerSymbolTableEntry* m_pSymbolQueryList;
    int32_t m_nSymbolLabelListSize;
    int32_t m_nSymbolLabelList;
    CScriptCompilerSymbolTableEntry* m_pSymbolLabelList;
    int32_t m_pSymbolLabelStartEntry[512];
    int32_t m_nDebugSymbolicOutput;
    int32_t m_nGenerateDebuggerOutput;
    int32_t m_bAutomaticCleanUpAfterCompiles;
    int32_t m_bOptimizeBinarySpace;
    int32_t m_nTotalCompileNodes;
    int32_t m_bCompilingConditional;
    char* m_pchOutputCode;
    int32_t m_nOutputCodeSize;
    int32_t m_nOutputCodeLength;
    int32_t m_nBinaryCodeLength;
    char* m_pchResolvedOutputBuffer;
    int32_t m_nResolvedOutputBufferSize;
    char* m_pchDebuggerCode;
    int32_t m_nDebuggerCodeSize;
    int32_t m_nDebuggerCodeLength;
    char m_pchActionParameters[32];
    CExoString m_pchActionParameterStructureNames[32];
    int32_t m_nFinalBinarySize;
    CExoString m_sCapturedError;

    // The below are auto generated stubs.
    CScriptCompiler(const CScriptCompiler&) = default;
    CScriptCompiler& operator=(const CScriptCompiler&) = default;

    CScriptCompiler();
    ~CScriptCompiler();
    void AddStructureToStack(const CExoString&, int32_t);
    int32_t AddSymbolToLabelList(int32_t, int32_t, int32_t, int32_t);
    int32_t AddSymbolToQueryList(int32_t, int32_t, int32_t, int32_t);
    int32_t AddToGlobalVariableList(CScriptParseTreeNode*);
    void AddToSymbolTableVarStack(int32_t, int32_t, int32_t);
    int32_t AddUserDefinedIdentifier(CScriptParseTreeNode*, int32_t);
    void AddVariableToStack(int32_t, CExoString*, int32_t);
    int32_t CheckForBadLValue(CScriptParseTreeNode*);
    int32_t CleanUpAfterCompile(int32_t, CScriptParseTreeNode*);
    void CleanUpAfterCompiles();
    int32_t CleanUpDuringCompile(int32_t);
    void ClearAllSymbolLists();
    void ClearCompiledScriptCode();
    void ClearSwitchLabelList();
    void ClearUserDefinedIdentifiers();
    int32_t CompileFile(const CExoString&);
    int32_t CompileScriptChunk(const CExoString&);
    int32_t CompileScriptConditional(const CExoString&);
    CScriptParseTreeNode* CreateScriptParseTreeNode(int32_t, CScriptParseTreeNode*, CScriptParseTreeNode*);
    void DeleteCompileStack();
    void DeleteParseTree(int32_t, CScriptParseTreeNode*);
    void DeleteScriptParseTreeNode(CScriptParseTreeNode*);
    int32_t DetermineLocationOfCode();
    CScriptParseTreeNode* DuplicateScriptParseTree(CScriptParseTreeNode*);
    void EndLineNumberAtBinaryInstruction(int32_t, int32_t, int32_t);
    void FinalizeFinalCode();
    int32_t FoundReturnStatementOnAllBranches(CScriptParseTreeNode*);
    int32_t GenerateCodeForSwitchLabels(CScriptParseTreeNode*);
    CExoString GenerateDebuggerTypeAbbreviation(int32_t, CExoString);
    int32_t GenerateFinalCodeFromParseTree(CExoString);
    int32_t GenerateIdentifierList();
    int32_t GenerateIdentifiersFromConstantVariables(CScriptParseTreeNode*);
    int32_t GenerateParseTree();
    int32_t GetCompiledScriptCode(char**, int32_t*);
    CExoString GetFunctionNameFromSymbolSubTypes(int32_t, int32_t);
    int32_t GetHashEntryByName(const char*);
    int32_t GetIdentifierByName(const CExoString&);
    CScriptParseTreeNode* GetNewScriptParseTreeNode();
    int32_t GetStructureField(const CExoString&, const CExoString&);
    int32_t GetStructureSize(const CExoString&);
    int32_t HandleIdentifierToken();
    int32_t HandleToken();
    uint32_t HashManagerAdd(uint32_t, uint32_t);
    uint32_t HashManagerDelete(uint32_t, uint32_t);
    uint32_t HashString(const CExoString&);
    uint32_t HashString(const char*);
    void Initialize();
    void InitializeFinalCode();
    void InitializeIncludeFile(int32_t);
    void InitializePreDefinedStructures();
    void InitializeSwitchLabelList();
    CScriptParseTreeNode* InsertGlobalVariablesInParseTree(CScriptParseTreeNode*);
    int32_t InstallLoader();
    int32_t InVisitGenerateCode(CScriptParseTreeNode*);
    void ModifySRStackReturnTree(CScriptParseTreeNode*);
    int32_t OutputError(int32_t, CExoString*, int32_t, const CExoString&);
    int32_t OutputIdentifierError(const CExoString&, int32_t, int32_t);
    int32_t OutputWalkTreeError(int32_t, CScriptParseTreeNode*);
    int32_t ParseCharacterAlphabet(int32_t);
    int32_t ParseCharacterAmpersand(int32_t);
    int32_t ParseCharacterAsterisk(int32_t);
    int32_t ParseCharacterCarat(int32_t);
    int32_t ParseCharacterColon();
    int32_t ParseCharacterComma();
    int32_t ParseCharacterEllipsis();
    int32_t ParseCharacterEqualSign(int32_t);
    int32_t ParseCharacterExclamationPoint(int32_t);
    int32_t ParseCharacterHyphen(int32_t);
    int32_t ParseCharacterLeftAngle(int32_t);
    int32_t ParseCharacterLeftBrace();
    int32_t ParseCharacterLeftBracket();
    int32_t ParseCharacterLeftSquareBracket();
    int32_t ParseCharacterNumeric(int32_t);
    int32_t ParseCharacterPercentSign(int32_t);
    int32_t ParseCharacterPeriod();
    int32_t ParseCharacterPlusSign(int32_t);
    int32_t ParseCharacterQuestionMark();
    int32_t ParseCharacterQuotationMark();
    int32_t ParseCharacterRightAngle(int32_t);
    int32_t ParseCharacterRightBrace();
    int32_t ParseCharacterRightBracket();
    int32_t ParseCharacterRightSquareBracket();
    int32_t ParseCharacterSemicolon();
    int32_t ParseCharacterSlash(int32_t);
    int32_t ParseCharacterTilde();
    int32_t ParseCharacterVerticalBar(int32_t);
    int32_t ParseCommentedOutCharacter(int32_t);
    float ParseFloatFromTokenString();
    int32_t ParseIdentifierFile();
    int32_t ParseNextCharacter(int32_t, int32_t);
    int32_t ParseSource(char*, int32_t);
    int32_t ParseStringCharacter(int32_t, int32_t);
    int32_t PopSRStack(int32_t*, int32_t*, int32_t*, CScriptParseTreeNode**, CScriptParseTreeNode**);
    int32_t PostVisitGenerateCode(CScriptParseTreeNode*);
    int32_t PreVisitGenerateCode(CScriptParseTreeNode*);
    void PrintBinaryAddress();
    int32_t PrintParseIdentifierFileError(int32_t);
    int32_t PrintParseSourceError(int32_t);
    void PushSRStack(int32_t, int32_t, int32_t, CScriptParseTreeNode*);
    void RemoveFromSymbolTableVarStack(int32_t, int32_t, int32_t);
    void ResolveDebuggingInformation();
    void ResolveDebuggingInformationForIdentifier(int32_t);
    int32_t ResolveLabels();
    void SetAutomaticCleanUpAfterCompiles(int32_t);
    void SetCompileConditionalFile(int32_t);
    void SetCompileConditionalOrMain(int32_t);
    void SetCompileDebugLevel(int32_t);
    void SetCompileSymbolicOutput(int32_t);
    void SetGenerateDebuggerOutput(int32_t);
    void SetIdentifierSpecification(const CExoString&);
    void SetOptimizeBinaryCodeLength(int32_t);
    void SetOutputAlias(const CExoString&);
    void ShutDown();
    void ShutdownIncludeFile(int32_t);
    void StartLineNumberAtBinaryInstruction(int32_t, int32_t, int32_t);
    int32_t Test_CompareDirectoryContents(CExoString&, CExoString&);
    int32_t Test_CompareFileInclusion(CExoString&, CExoString&, CExoStringList*, uint16_t);
    void Test_CompileAllScriptsInDirectory(CExoString&, CExoString&, CExoString&);
    int32_t TestIdentifierToken();
    void TokenInitialize();
    int32_t TraverseTreeForSwitchLabels(CScriptParseTreeNode*);
    int32_t ValidateLocationOfIdentifier(const CExoString&);
    int32_t WalkParseTree(CScriptParseTreeNode*);
    int32_t WriteDebuggerOutputToFile(CExoString);
    int32_t WriteFinalCodeToFile(const CExoString&);
    int32_t WriteResolvedOutput();
};

void CScriptCompiler__CScriptCompilerCtor__0(CScriptCompiler* thisPtr);
void CScriptCompiler__CScriptCompilerDtor__0(CScriptCompiler* thisPtr);
void CScriptCompiler__AddStructureToStack(CScriptCompiler* thisPtr, const CExoString&, int32_t);
int32_t CScriptCompiler__AddSymbolToLabelList(CScriptCompiler* thisPtr, int32_t, int32_t, int32_t, int32_t);
int32_t CScriptCompiler__AddSymbolToQueryList(CScriptCompiler* thisPtr, int32_t, int32_t, int32_t, int32_t);
int32_t CScriptCompiler__AddToGlobalVariableList(CScriptCompiler* thisPtr, CScriptParseTreeNode*);
void CScriptCompiler__AddToSymbolTableVarStack(CScriptCompiler* thisPtr, int32_t, int32_t, int32_t);
int32_t CScriptCompiler__AddUserDefinedIdentifier(CScriptCompiler* thisPtr, CScriptParseTreeNode*, int32_t);
void CScriptCompiler__AddVariableToStack(CScriptCompiler* thisPtr, int32_t, CExoString*, int32_t);
int32_t CScriptCompiler__CheckForBadLValue(CScriptCompiler* thisPtr, CScriptParseTreeNode*);
int32_t CScriptCompiler__CleanUpAfterCompile(CScriptCompiler* thisPtr, int32_t, CScriptParseTreeNode*);
void CScriptCompiler__CleanUpAfterCompiles(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__CleanUpDuringCompile(CScriptCompiler* thisPtr, int32_t);
void CScriptCompiler__ClearAllSymbolLists(CScriptCompiler* thisPtr);
void CScriptCompiler__ClearCompiledScriptCode(CScriptCompiler* thisPtr);
void CScriptCompiler__ClearSwitchLabelList(CScriptCompiler* thisPtr);
void CScriptCompiler__ClearUserDefinedIdentifiers(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__CompileFile(CScriptCompiler* thisPtr, const CExoString&);
int32_t CScriptCompiler__CompileScriptChunk(CScriptCompiler* thisPtr, const CExoString&);
int32_t CScriptCompiler__CompileScriptConditional(CScriptCompiler* thisPtr, const CExoString&);
CScriptParseTreeNode* CScriptCompiler__CreateScriptParseTreeNode(CScriptCompiler* thisPtr, int32_t, CScriptParseTreeNode*, CScriptParseTreeNode*);
void CScriptCompiler__DeleteCompileStack(CScriptCompiler* thisPtr);
void CScriptCompiler__DeleteParseTree(CScriptCompiler* thisPtr, int32_t, CScriptParseTreeNode*);
void CScriptCompiler__DeleteScriptParseTreeNode(CScriptCompiler* thisPtr, CScriptParseTreeNode*);
int32_t CScriptCompiler__DetermineLocationOfCode(CScriptCompiler* thisPtr);
CScriptParseTreeNode* CScriptCompiler__DuplicateScriptParseTree(CScriptCompiler* thisPtr, CScriptParseTreeNode*);
void CScriptCompiler__EndLineNumberAtBinaryInstruction(CScriptCompiler* thisPtr, int32_t, int32_t, int32_t);
void CScriptCompiler__FinalizeFinalCode(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__FoundReturnStatementOnAllBranches(CScriptCompiler* thisPtr, CScriptParseTreeNode*);
int32_t CScriptCompiler__GenerateCodeForSwitchLabels(CScriptCompiler* thisPtr, CScriptParseTreeNode*);
CExoString CScriptCompiler__GenerateDebuggerTypeAbbreviation(CScriptCompiler* thisPtr, int32_t, CExoString);
int32_t CScriptCompiler__GenerateFinalCodeFromParseTree(CScriptCompiler* thisPtr, CExoString);
int32_t CScriptCompiler__GenerateIdentifierList(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__GenerateIdentifiersFromConstantVariables(CScriptCompiler* thisPtr, CScriptParseTreeNode*);
int32_t CScriptCompiler__GenerateParseTree(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__GetCompiledScriptCode(CScriptCompiler* thisPtr, char**, int32_t*);
CExoString CScriptCompiler__GetFunctionNameFromSymbolSubTypes(CScriptCompiler* thisPtr, int32_t, int32_t);
int32_t CScriptCompiler__GetHashEntryByName(CScriptCompiler* thisPtr, const char*);
int32_t CScriptCompiler__GetIdentifierByName(CScriptCompiler* thisPtr, const CExoString&);
CScriptParseTreeNode* CScriptCompiler__GetNewScriptParseTreeNode(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__GetStructureField(CScriptCompiler* thisPtr, const CExoString&, const CExoString&);
int32_t CScriptCompiler__GetStructureSize(CScriptCompiler* thisPtr, const CExoString&);
int32_t CScriptCompiler__HandleIdentifierToken(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__HandleToken(CScriptCompiler* thisPtr);
uint32_t CScriptCompiler__HashManagerAdd(CScriptCompiler* thisPtr, uint32_t, uint32_t);
uint32_t CScriptCompiler__HashManagerDelete(CScriptCompiler* thisPtr, uint32_t, uint32_t);
uint32_t CScriptCompiler__HashString__0(CScriptCompiler* thisPtr, const CExoString&);
uint32_t CScriptCompiler__HashString__1(CScriptCompiler* thisPtr, const char*);
void CScriptCompiler__Initialize(CScriptCompiler* thisPtr);
void CScriptCompiler__InitializeFinalCode(CScriptCompiler* thisPtr);
void CScriptCompiler__InitializeIncludeFile(CScriptCompiler* thisPtr, int32_t);
void CScriptCompiler__InitializePreDefinedStructures(CScriptCompiler* thisPtr);
void CScriptCompiler__InitializeSwitchLabelList(CScriptCompiler* thisPtr);
CScriptParseTreeNode* CScriptCompiler__InsertGlobalVariablesInParseTree(CScriptCompiler* thisPtr, CScriptParseTreeNode*);
int32_t CScriptCompiler__InstallLoader(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__InVisitGenerateCode(CScriptCompiler* thisPtr, CScriptParseTreeNode*);
void CScriptCompiler__ModifySRStackReturnTree(CScriptCompiler* thisPtr, CScriptParseTreeNode*);
int32_t CScriptCompiler__OutputError(CScriptCompiler* thisPtr, int32_t, CExoString*, int32_t, const CExoString&);
int32_t CScriptCompiler__OutputIdentifierError(CScriptCompiler* thisPtr, const CExoString&, int32_t, int32_t);
int32_t CScriptCompiler__OutputWalkTreeError(CScriptCompiler* thisPtr, int32_t, CScriptParseTreeNode*);
int32_t CScriptCompiler__ParseCharacterAlphabet(CScriptCompiler* thisPtr, int32_t);
int32_t CScriptCompiler__ParseCharacterAmpersand(CScriptCompiler* thisPtr, int32_t);
int32_t CScriptCompiler__ParseCharacterAsterisk(CScriptCompiler* thisPtr, int32_t);
int32_t CScriptCompiler__ParseCharacterCarat(CScriptCompiler* thisPtr, int32_t);
int32_t CScriptCompiler__ParseCharacterColon(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__ParseCharacterComma(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__ParseCharacterEllipsis(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__ParseCharacterEqualSign(CScriptCompiler* thisPtr, int32_t);
int32_t CScriptCompiler__ParseCharacterExclamationPoint(CScriptCompiler* thisPtr, int32_t);
int32_t CScriptCompiler__ParseCharacterHyphen(CScriptCompiler* thisPtr, int32_t);
int32_t CScriptCompiler__ParseCharacterLeftAngle(CScriptCompiler* thisPtr, int32_t);
int32_t CScriptCompiler__ParseCharacterLeftBrace(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__ParseCharacterLeftBracket(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__ParseCharacterLeftSquareBracket(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__ParseCharacterNumeric(CScriptCompiler* thisPtr, int32_t);
int32_t CScriptCompiler__ParseCharacterPercentSign(CScriptCompiler* thisPtr, int32_t);
int32_t CScriptCompiler__ParseCharacterPeriod(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__ParseCharacterPlusSign(CScriptCompiler* thisPtr, int32_t);
int32_t CScriptCompiler__ParseCharacterQuestionMark(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__ParseCharacterQuotationMark(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__ParseCharacterRightAngle(CScriptCompiler* thisPtr, int32_t);
int32_t CScriptCompiler__ParseCharacterRightBrace(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__ParseCharacterRightBracket(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__ParseCharacterRightSquareBracket(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__ParseCharacterSemicolon(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__ParseCharacterSlash(CScriptCompiler* thisPtr, int32_t);
int32_t CScriptCompiler__ParseCharacterTilde(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__ParseCharacterVerticalBar(CScriptCompiler* thisPtr, int32_t);
int32_t CScriptCompiler__ParseCommentedOutCharacter(CScriptCompiler* thisPtr, int32_t);
float CScriptCompiler__ParseFloatFromTokenString(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__ParseIdentifierFile(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__ParseNextCharacter(CScriptCompiler* thisPtr, int32_t, int32_t);
int32_t CScriptCompiler__ParseSource(CScriptCompiler* thisPtr, char*, int32_t);
int32_t CScriptCompiler__ParseStringCharacter(CScriptCompiler* thisPtr, int32_t, int32_t);
int32_t CScriptCompiler__PopSRStack(CScriptCompiler* thisPtr, int32_t*, int32_t*, int32_t*, CScriptParseTreeNode**, CScriptParseTreeNode**);
int32_t CScriptCompiler__PostVisitGenerateCode(CScriptCompiler* thisPtr, CScriptParseTreeNode*);
int32_t CScriptCompiler__PreVisitGenerateCode(CScriptCompiler* thisPtr, CScriptParseTreeNode*);
void CScriptCompiler__PrintBinaryAddress(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__PrintParseIdentifierFileError(CScriptCompiler* thisPtr, int32_t);
int32_t CScriptCompiler__PrintParseSourceError(CScriptCompiler* thisPtr, int32_t);
void CScriptCompiler__PushSRStack(CScriptCompiler* thisPtr, int32_t, int32_t, int32_t, CScriptParseTreeNode*);
void CScriptCompiler__RemoveFromSymbolTableVarStack(CScriptCompiler* thisPtr, int32_t, int32_t, int32_t);
void CScriptCompiler__ResolveDebuggingInformation(CScriptCompiler* thisPtr);
void CScriptCompiler__ResolveDebuggingInformationForIdentifier(CScriptCompiler* thisPtr, int32_t);
int32_t CScriptCompiler__ResolveLabels(CScriptCompiler* thisPtr);
void CScriptCompiler__SetAutomaticCleanUpAfterCompiles(CScriptCompiler* thisPtr, int32_t);
void CScriptCompiler__SetCompileConditionalFile(CScriptCompiler* thisPtr, int32_t);
void CScriptCompiler__SetCompileConditionalOrMain(CScriptCompiler* thisPtr, int32_t);
void CScriptCompiler__SetCompileDebugLevel(CScriptCompiler* thisPtr, int32_t);
void CScriptCompiler__SetCompileSymbolicOutput(CScriptCompiler* thisPtr, int32_t);
void CScriptCompiler__SetGenerateDebuggerOutput(CScriptCompiler* thisPtr, int32_t);
void CScriptCompiler__SetIdentifierSpecification(CScriptCompiler* thisPtr, const CExoString&);
void CScriptCompiler__SetOptimizeBinaryCodeLength(CScriptCompiler* thisPtr, int32_t);
void CScriptCompiler__SetOutputAlias(CScriptCompiler* thisPtr, const CExoString&);
void CScriptCompiler__ShutDown(CScriptCompiler* thisPtr);
void CScriptCompiler__ShutdownIncludeFile(CScriptCompiler* thisPtr, int32_t);
void CScriptCompiler__StartLineNumberAtBinaryInstruction(CScriptCompiler* thisPtr, int32_t, int32_t, int32_t);
int32_t CScriptCompiler__Test_CompareDirectoryContents(CScriptCompiler* thisPtr, CExoString&, CExoString&);
int32_t CScriptCompiler__Test_CompareFileInclusion(CScriptCompiler* thisPtr, CExoString&, CExoString&, CExoStringList*, uint16_t);
void CScriptCompiler__Test_CompileAllScriptsInDirectory(CScriptCompiler* thisPtr, CExoString&, CExoString&, CExoString&);
int32_t CScriptCompiler__TestIdentifierToken(CScriptCompiler* thisPtr);
void CScriptCompiler__TokenInitialize(CScriptCompiler* thisPtr);
int32_t CScriptCompiler__TraverseTreeForSwitchLabels(CScriptCompiler* thisPtr, CScriptParseTreeNode*);
int32_t CScriptCompiler__ValidateLocationOfIdentifier(CScriptCompiler* thisPtr, const CExoString&);
int32_t CScriptCompiler__WalkParseTree(CScriptCompiler* thisPtr, CScriptParseTreeNode*);
int32_t CScriptCompiler__WriteDebuggerOutputToFile(CScriptCompiler* thisPtr, CExoString);
int32_t CScriptCompiler__WriteFinalCodeToFile(CScriptCompiler* thisPtr, const CExoString&);
int32_t CScriptCompiler__WriteResolvedOutput(CScriptCompiler* thisPtr);

}

}
