# Installer sections

Section "Zensur" SEC0000

    SectionIn RO
    SetOutPath $INSTDIR
    SetOverwrite on

    File ..\Zensur\Release\Zensur.exe

    File License.txt
    
    !insertmacro MUI_STARTMENU_WRITE_BEGIN StartMenuSelection
    CreateDirectory "$SMPROGRAMS\$StartMenuGroup"
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\Uninstall $(^Name).lnk" "$INSTDIR\Uninstall $(^Name).exe"
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\$(^Name).lnk" $INSTDIR\$(^Name).exe
    !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

Section -post SEC0001
    WriteRegStr HKCU "${REGKEY}" InstallPath $INSTDIR
    WriteUninstaller "$INSTDIR\Uninstall $(^Name).exe"

    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayName "$(^Name)"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayVersion "${VERSION}"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" Publisher "${COMPANY}"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" URLInfoAbout "${URL}"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayIcon "$INSTDIR\Uninstall Zensur.exe"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" UninstallString "$INSTDIR\Uninstall Zensur.exe"
    WriteRegDWORD HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" NoModify 1
    WriteRegDWORD HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" NoRepair 1
SectionEnd