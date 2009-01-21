#define BuildMode "opt"
#define AppName "mites"
#define AppVerName "mites 1.0"
#define Publisher "CHDS"
#define AppURL "http://svndmz.nps.edu"
#define AppExeName "mites.exe"
#define DependDir "..\..\..\depend"
#define VsDir "c:\Program Files\Microsoft Visual Studio 8"

[Setup]
AppId={{8157B288-7994-4089-9D9D-E929946CA9AA}
AppName={#AppName}
AppVerName={#AppVerName}
AppPublisher={#Publisher}
AppPublisherURL={#AppURL}
AppSupportURL={#AppURL}
AppUpdatesURL={#AppURL}
ShowLanguageDialog=no
DefaultDirName={pf}\{#AppName}
DefaultGroupName={#AppName}
UninstallDisplayName={#AppName}
UninstallDisplayIcon={app}\bin\{#AppExeName}
UninstallFilesDir={app}\uninst
OutputDir=.
OutputBaseFilename=mitessetup
;OutputManifestFile=manifest.txt
;WizardImageFile=WizImage.bmp
;WizardSmallImageFile=WizSmallImage.bmp
;SetupIconFile=Setup.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Languages:
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Icons]
Name: {group}\{#AppName}; Filename: {app}\bin\{#AppExeName}; WorkingDir: {app}; IconFilename: {app}\bin\{#AppExeName}; Tasks:
Name: {group}\{cm:UninstallProgram,{#AppName}}; Filename: {uninstallexe}
Name: {userdesktop}\{#AppName}; Filename: {app}\bin\{#AppExeName}; Tasks: desktopicon; WorkingDir: {app}; IconFilename: {app}\bin\{#AppExeName}; Languages:
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\{#AppName}; Filename: {app}\bin\{#AppExeName}; Tasks: quicklaunchicon; WorkingDir: {app}; IconFilename: {app}\bin\{#AppExeName}; Languages:

[Run]
Filename: {app}\bin\{#AppExeName}; Description: {cm:LaunchProgram,{#AppName}}; Flags: nowait postinstall skipifsilent; Tasks: ; Languages:

[Registry]
Root: HKLM; Subkey: Software\DMZ\{#AppName}; ValueType: string; ValueName: workingDir; ValueData: {app}; Flags: uninsdeletekey
Root: HKCR; Subkey: mites.file\shell\open\command; ValueType: string; ValueData: "{app}\bin\mites.exe ""%1"""; Flags: uninsdeletekey

[Files]
Source: ..\..\..\bin\win32-{#BuildMode}\mites.app\*; DestDir: {app}; Flags: recursesubdirs
Source: {#DependDir}\bin\QtCore4.dll; DestDir: {app}\bin
Source: {#DependDir}\bin\QtGui4.dll; DestDir: {app}\bin
Source: {#DependDir}\bin\QtOpenGL4.dll; DestDir: {app}\bin
Source: {#DependDir}\bin\QtSvg4.dll; DestDir: {app}\bin
Source: {#DependDir}\bin\QtXml4.dll; DestDir: {app}\bin
Source: {#VsDir}\VC\redist\x86\Microsoft.VC80.CRT\*; DestDir: {app}\bin\Microsoft.VC80.CRT
