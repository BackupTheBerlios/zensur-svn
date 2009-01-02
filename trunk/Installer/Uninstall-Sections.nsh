# Uninstaller sections
Section "Un.Zensur)" UNSEC0000
File ..\Zensur\Release\Zensur.exe
    Delete $INSTDIR\Lizenz.txt
    Delete $INSTDIR\Zensur.exe
SectionEnd

Section un.post UNSEC0001

    !insertmacro MUI_STARTMENU_GETFOLDER StartMenuSelection $R0
    Delete "$SMPROGRAMS\$R0\$(^Name).lnk"
    Delete "$SMPROGRAMS\$R0\$(^Name) deinstallieren.lnk"
    RMDir $SMPROGRAMS\$R0

    Delete "$INSTDIR\$(^Name) deinstallieren.exe"
    RMDir $INSTDIR

    DeleteRegValue HKCU "${REGKEY}" StartMenuGroup
    DeleteRegValue HKCU "${REGKEY}" InstallPath

    DeleteRegKey /IfEmpty HKCU "${REGKEY}"
    
    DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)"

SectionEnd