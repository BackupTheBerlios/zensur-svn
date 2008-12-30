# Variables
Var StartMenuGroup

# MUI defines
!define MUI_ICON "Graphics\Install.ico"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "Graphics\Header.bmp"
!define MUI_WELCOMEPAGE_TEXT \
  "Das Programm Zensur richtet sich an Schüler und Lehrer. \
   Es ermöglicht die einfache und schnelle Berechnung von Endnoten.\r\n\r\n\
   Dieser Assistent wird Sie durch die Installation begleiten. \
   \r\n\r\n$_CLICK"
!define MUI_WELCOMEFINISHPAGE_BITMAP "Graphics\Welcome.bmp"
!define MUI_PAGE_HEADER_SUBTEXT \
  "Diese Software ist unter der GNU General Public License \
   (GPL) lizensiert. Sie können sie ändern und weiterzugeben."
!define MUI_LICENSEPAGE_TEXT_BOTTOM \
  "In aller Kürze: Sie haben das Recht, die Software zu jedem Zweck \
   einzusetzen. Sie können die Software weitergeben, sofern Sie dem \
   Empfänger dieselben Rechte einräumen, die auch Sie erhalten haben."
!define MUI_COMPONENTSPAGE_SMALLDESC
!define MUI_STARTMENUPAGE_REGISTRY_ROOT HKCU
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\$(^Name)"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME StartMenuGroup
!define MUI_STARTMENUPAGE_DEFAULT_FOLDER "$(^Name)"
!define MUI_FINISHPAGE_LINK "Zur Website mit Neuigkeiten und Hilfsangeboten"
!define MUI_FINISHPAGE_LINK_LOCATION "http://zensur.berlios.de"
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_UNICON "Graphics\Uninstall.ico"
!define MUI_UNFINISHPAGE_NOAUTOCLOSE

# Installer pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE License.txt
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_STARTMENU StartMenuSelection $StartMenuGroup
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

# Installer languages
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "English"