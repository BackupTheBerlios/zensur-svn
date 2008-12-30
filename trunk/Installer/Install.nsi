# Included files of NSIS
!include LogicLib.nsh
!include MUI.nsh
!include Sections.nsh

!include "Project.nsh"

# Reserved Files

# Installer attributes
OutFile "${OUT_FILE_NAME}.exe"
InstallDir "$PROGRAMFILES\$(^Name)"
CRCCheck on
XPStyle on
ShowInstDetails show
VIProductVersion "${VERSION}.0.0"
VIAddVersionKey ProductName "${Name}"
VIAddVersionKey ProductVersion "${VERSION}"
VIAddVersionKey CompanyName "${COMPANY}"
VIAddVersionKey CompanyWebsite "${URL}"
VIAddVersionKey LegalCopyright ""
VIAddVersionKey FileVersion "${VERSION}"
VIAddVersionKey FileDescription "${Name} Installer"
InstallDirRegKey HKLM "${REGKEY}" InstallPath
ShowUninstDetails show

# Modern User Interface including global defines, pages and settings
!include "Modern-UI.nsh"

# Installer sections
!include "Install-Sections.nsh"

# Uninstaller sections
!include "Uninstall-Sections.nsh"