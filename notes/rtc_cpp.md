Run Time Compiled C++ Notes

## Doc
http://www.gameaipro.com/GameAIPro/GameAIPro_Chapter15_Runtime_Compiled_C++_for_Rapid_AI_Development.pdf

## Code

https://github.com/RuntimeCompiledCPlusPlus/RuntimeCompiledCPlusPlus

* Classes that will be re-compiled should inherit from IObject
* IRuntimeObjectSystem can invoke recompiles
* ICompilerLogger - logged output from compiler
* IObjectFactoryListener - ??

## Examples

### Console Examples

`ConsoleGame` is initialized, and starts looping.
`ConsoleGame` inherits from IObjectFactoryListener
    IObjectFactoryListener provides OnConstructorsAdded()
    ConsoleGame has some private systems
        ICompilerLogger - Needed for...something
        IRuntimeObjectSystem
        IUpdateable
        ObjectId
    
ConsoleGame::Init
    Creates new _concrete_ RuntimeObjectSystem
    CompilerLogger concrete implementation goes to stdio

ICompilerLogger
    LogError, LogWarning, LogInfo, destructor
StdioLogSystem : ICompilerLogger
    Pipes interface methods to LogInternal
    LogInternal prints to fixed size buffer, couts the buffer (and to win debug)

ITestBuildNotifier
    TestBuildCallback, TestBuildWaitAndUpdate
IRuntimeObjectSystem : ITestBuildNotifier
    // Seems like this interface 
    Initialize - Receives ICompilerLogger + SystemTable
    GetIsCompiling, GetIsCompiledComplete, LoadCompiledModule, GetLastLoadModuleSuccess
    GetNumberLoadedModules
    GetObjectFactorySystem
    GetFileChangeNotifier, GetLogger
    CompileAll
    CompileAllInProject // Compiler bits can be specified as 'projects'!
    AddToRuntimeFileList, RemoveFromRuntimeFileList
    AddIncludeDir, AddLibraryDir
    SetAdditionalCompileOptions, SetAdditionalLinkOptions
    SetCompilerLocation
    SetOptimizationLevel, GetOptimizationLevel
    SetIntermediateDir
    SetAutoCompile, GetAutoCompile
    SetFastCompileMode
    CleanObjectFiles
    SetupObjectConstructors
    SetProtectionEnabled, IsProtectionEnabled, TryProtectedFunction
    TestBuildAllRuntimeSourceFiles, TestBuildAllRuntimeHeaders
    FindFile, AddPathToSourceSearch
IFileChangeListener
    OnFileChange, OnRegisteredWithNotifier
RuntimeObjectSystem : IRuntimeObjectSystem, IFileChangeListener
SystemTable - table of systems that work together, probably used to register RuntimeObjectSystem

RuntimeObjectSystem has a FileChangeNotifier that is platform dependendent!
    Along with some controls about how the file monitoring and notifications are processed
RuntimeObjectSystem owns the BuildTool. BuildTool owns the Compiler (member)
RuntimeObjectSystem constructs a platform implementation of...?

Each object registers with a per module thing
Permodule tied to runtime object system


## Integration

Include these 'projects':
* RuntimeCompiler
* RuntimeObjectSystem

* Import projects to track objects and compile
* Objects to recompile derive from IObject
* Main engine class inherits from IObjectFactoryListener 