; Script de instalacao para as distribuicoes C++/Qt do AGENDA

#define MyAppName "AGENDA AVALIACOES ACADEMICAS"
#define MyAppVersion "2026.4.24.0"
#define MyAppPublisher "Fernando Nillsson Cidade"
#define MyAppURL "https://github.com/fernandoncidade"
#define MyAppExeName "AgendaAvaliacoesAcademicas.exe"
#define ScriptPath AddBackslash(SourcePath) + "Agenda_Avaliacoes_Academicas.iss"
#define IsccPath AddBackslash(CompilerPath) + "ISCC.exe"

#ifndef BuildMSVC
  #ifndef BuildMinGW
    #define BuildMinGWExitCode Exec(IsccPath, "/DBuildMinGW " + AddQuotes(ScriptPath), SourcePath)
    #if BuildMinGWExitCode != 0
      #error A compilacao do instalador MinGW falhou.
    #endif
    #define BuildMSVC
    #pragma message "Compilacao orquestrada: MinGW concluida; prosseguindo com MSVC."
  #endif
#endif

#if Defined(BuildMSVC)
  #define MyBuildSuffix "msvc"
  #define MyDistDir "D:\MISCELANEAS\VSCode\AGENDA\AgendaAvaliacoesAcademicas_msvc"
  #define MyOutputDir "D:\MISCELANEAS\VSCode\AGENDA\InnoSetupAgenda_msvc"
#else
  #define MyBuildSuffix "mingw"
  #define MyDistDir "D:\MISCELANEAS\VSCode\AGENDA\AgendaAvaliacoesAcademicas_mingw"
  #define MyOutputDir "D:\MISCELANEAS\VSCode\AGENDA\InnoSetupAgenda_mingw"
#endif

#define MyOutputBaseFilename "AgendaAvaliacoesAcademicas_" + MyBuildSuffix + "_v" + MyAppVersion
#define MyAboutPtBrFile AddBackslash(MyDistDir) + "source\assets\ABOUT\ABOUT_pt_BR.txt"
#define MyAboutEnUsFile AddBackslash(MyDistDir) + "source\assets\ABOUT\ABOUT_en_US.txt"
#define MyPrivacyPtBrFile AddBackslash(MyDistDir) + "source\assets\PRIVACY_POLICY\Privacy_Policy_pt_BR.txt"
#define MyPrivacyEnUsFile AddBackslash(MyDistDir) + "source\assets\PRIVACY_POLICY\Privacy_Policy_en_US.txt"
#define MySetupIconFile AddBackslash(MyDistDir) + "source\assets\icones\ReviewsManager.ico"
#define MyMainExeFile AddBackslash(MyDistDir) + MyAppExeName

#if Defined(BuildMSVC) || Defined(BuildMinGW)
  #if !DirExists(MyDistDir)
    #error A distribuicao externa nao foi encontrada.
  #endif
  #if !FileExists(MyMainExeFile)
    #error O executavel principal nao foi encontrado na distribuicao externa.
  #endif
  #if !FileExists(MyAboutPtBrFile) || !FileExists(MyAboutEnUsFile)
    #error Os arquivos ABOUT nao foram encontrados na distribuicao externa.
  #endif
  #if !FileExists(MyPrivacyPtBrFile) || !FileExists(MyPrivacyEnUsFile)
    #error Os arquivos de politica de privacidade nao foram encontrados na distribuicao externa.
  #endif
  #if !FileExists(MySetupIconFile)
    #error O icone do instalador nao foi encontrado na distribuicao externa.
  #endif
#endif

[Setup]
AppId={{02BCC200-EEB3-4C06-BB64-00789CFD34AB}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
UninstallDisplayName={#MyAppName}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
UninstallDisplayIcon={app}\{#MyAppExeName}
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
DisableProgramGroupPage=yes
InfoBeforeFile={#MyAboutEnUsFile}
LicenseFile={#MyPrivacyEnUsFile}
OutputDir={#MyOutputDir}
OutputBaseFilename={#MyOutputBaseFilename}
SetupIconFile={#MySetupIconFile}
SolidCompression=yes
WizardStyle=modern
ShowLanguageDialog=yes
AllowNoIcons=yes
DisableReadyPage=yes
DisableFinishedPage=no

[Languages]
Name: "brazilianportuguese"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"; InfoBeforeFile: "{#MyAboutPtBrFile}"; LicenseFile: "{#MyPrivacyPtBrFile}"
Name: "english"; MessagesFile: "compiler:Default.isl"; InfoBeforeFile: "{#MyAboutEnUsFile}"; LicenseFile: "{#MyPrivacyEnUsFile}"

[CustomMessages]
brazilianportuguese.AppLanguage=Idioma do aplicativo
brazilianportuguese.SelectAppLang=Selecione o idioma padrao do aplicativo
brazilianportuguese.Portuguese=Portugues
brazilianportuguese.English=Ingles

english.AppLanguage=Application language
english.SelectAppLang=Select the default application language
english.Portuguese=Portuguese
english.English=English

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "langpt_BR"; Description: "{cm:Portuguese}"; GroupDescription: "{cm:AppLanguage}"; Flags: exclusive
Name: "langen_US"; Description: "{cm:English}"; GroupDescription: "{cm:AppLanguage}"; Flags: exclusive unchecked

[Files]
Source: "{#MyDistDir}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon; WorkingDir: "{app}"

[Run]
Filename: "{sys}\icacls.exe"; Parameters: """{app}"" /grant *S-1-5-32-545:(OI)(CI)F"; Flags: runhidden; Description: "Configurando permissoes..."
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent; WorkingDir: "{app}"

[ReturnCodes]
6000=UserCancelled
6001=AppAlreadyExists
6002=AnotherInstallationRunning
6003=DiskSpaceFull
6004=RebootRequired
6005=NetworkFailure_DownloadError
6006=NetworkFailure_ConnectionLost
6007=PackageRejectedByPolicy
0=Success

[Code]
procedure CreateLanguageConfigJSON();
var
  FileName: string;
  LanguageCode: string;
  JSONContent: string;
  ConfigDir: string;
begin
  ConfigDir := ExpandConstant('{localappdata}\AgendaAvaliacoesAcademicas');
  FileName := ConfigDir + '\language.json';

  if not DirExists(ConfigDir) then
    ForceDirectories(ConfigDir);

  if WizardSilent then
    LanguageCode := 'en_US'
  else if WizardIsTaskSelected('langpt_BR') then
    LanguageCode := 'pt_BR'
  else if WizardIsTaskSelected('langen_US') then
    LanguageCode := 'en_US'
  else
    LanguageCode := 'pt_BR';

  JSONContent := '{' + #13#10;
  JSONContent := JSONContent + '  "idioma": "' + LanguageCode + '"' + #13#10;
  JSONContent := JSONContent + '}';

  SaveStringToFile(FileName, JSONContent, False);
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
    CreateLanguageConfigJSON();
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
var
  AppPath: string;
  LocalAppDataPath: string;
begin
  if CurUninstallStep = usUninstall then
  begin
    LocalAppDataPath := ExpandConstant('{localappdata}\AgendaAvaliacoesAcademicas');
    if DirExists(LocalAppDataPath) then
    begin
      Log('Removendo o diretorio de dados do aplicativo: ' + LocalAppDataPath);
      DelTree(LocalAppDataPath, True, True, True);
    end;

    AppPath := ExpandConstant('{app}');
    if DirExists(AppPath + '\_internal\logs') then
      DelTree(AppPath + '\_internal\logs', True, True, True);

    if DirExists(AppPath + '\_internal') then
      DelTree(AppPath + '\_internal', True, True, True);
  end;
end;
